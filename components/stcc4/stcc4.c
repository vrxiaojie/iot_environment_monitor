#include "stcc4.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY

#define LOG_TAG "STCC4"

static i2c_master_dev_handle_t stcc4_i2c_dev_handle = NULL;

esp_err_t stcc4_i2c_init(i2c_master_bus_handle_t bus_handle)
{
    i2c_device_config_t stcc4_i2c_dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = STCC4_I2C_ADDR_64,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &stcc4_i2c_dev_config, &stcc4_i2c_dev_handle));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 init successfully");
#endif
    return ESP_OK;
}

esp_err_t stcc4_i2c_deinit(i2c_master_bus_handle_t bus_handle)
{
    ESP_ERROR_CHECK(i2c_master_bus_rm_device(stcc4_i2c_dev_handle));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 de-init successfully");
#endif
    return ESP_OK;
}

// CRC8 check
static uint8_t crcCheck(uint8_t *ptr, uint8_t len)
{
    uint8_t crc = 0xFF;
    const uint8_t polynomial = 0x31;

    for (size_t i = 0; i < len; i++)
    {
        crc ^= ptr[i];
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ polynomial;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }
    return crc ^ 0x00; // Final XOR
}

static float signalTemperature(uint16_t rawTemperature)
{
    float temperature = 0.0;
    temperature = -45.0 + ((175.0 * rawTemperature) / 65535.0);
    return temperature;
}

static float signalRelativeHumidity(uint16_t rawRelativeHumidity)
{
    float relativeHumidity = 0.0;
    relativeHumidity = -6.0 + ((125.0 * rawRelativeHumidity) / 65535.0);
    return relativeHumidity;
}

esp_err_t stcc4_read_measurement(int16_t *co2Concentration, float *temperature,
                                 float *relativeHumidity, uint16_t *sensorStatus)
{
    int16_t rawCo2Concentration = 0;
    uint16_t rawTemperature = 0;
    uint16_t rawRelativeHumidity = 0;
    uint16_t sensorStatusRaw = 0;
    ESP_ERROR_CHECK(stcc4_read_measurement_raw(&rawCo2Concentration, &rawTemperature,
                                               &rawRelativeHumidity, &sensorStatusRaw));

    *co2Concentration = rawCo2Concentration;
    *temperature = signalTemperature(rawTemperature);
    *relativeHumidity = signalRelativeHumidity(rawRelativeHumidity);
    *sensorStatus = sensorStatusRaw;
    vTaskDelay(pdMS_TO_TICKS(1));
    return ESP_OK;
}

esp_err_t stcc4_start_continuous_measurement()
{
    uint8_t buf[2] = {0x21, 0x8b};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 start continuous measurement");
#endif
    return ESP_OK;
}

esp_err_t stcc4_stop_continuous_measurement()
{
    uint8_t buf[2] = {0x3f, 0x86};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 stop continuous measurement");
#endif
    vTaskDelay(pdMS_TO_TICKS(1200));
    return ESP_OK;
}

esp_err_t stcc4_read_measurement_raw(int16_t *co2ConcentrationRaw, uint16_t *temperatureRaw,
                                     uint16_t *relativeHumidityRaw, uint16_t *sensorStatusRaw)
{
    uint8_t buf[2] = {0xEC, 0x05};
    uint8_t receive_buf[12] = {0};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
    vTaskDelay(pdMS_TO_TICKS(1));

    i2c_master_receive(stcc4_i2c_dev_handle, receive_buf, sizeof(receive_buf), -1);
    *co2ConcentrationRaw = (int16_t)((receive_buf[0] << 8) | receive_buf[1]);
    if (crcCheck(&receive_buf[0], 2) != receive_buf[2])
    {
        ESP_LOGE(LOG_TAG, "CO2 CRC check failed");
        return ESP_FAIL;
    }
    *temperatureRaw = (uint16_t)((receive_buf[3] << 8) | receive_buf[4]);
    if (crcCheck(&receive_buf[0], 2) != receive_buf[2])
    {
        ESP_LOGE(LOG_TAG, "Temperature CRC check failed");
        return ESP_FAIL;
    }
    *relativeHumidityRaw = (uint16_t)((receive_buf[6] << 8) | receive_buf[7]);
    if (crcCheck(&receive_buf[0], 2) != receive_buf[2])
    {
        ESP_LOGE(LOG_TAG, "Humidity CRC check failed");
        return ESP_FAIL;
    }
    *sensorStatusRaw = (uint16_t)((receive_buf[9] << 8) | receive_buf[10]);
    if (crcCheck(&receive_buf[0], 2) != receive_buf[2])
    {
        ESP_LOGE(LOG_TAG, "Sensor status CRC check failed");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t stcc4_set_pressure_compensation(uint16_t pressure)
{
    pressure /= 2;
    uint8_t buf[5] = {0xe0, 0x16};
    buf[2] = (pressure >> 8) & 0xFF;
    buf[3] = pressure & 0xFF;
    buf[4] = crcCheck(&buf[2], 2);
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
    vTaskDelay(pdMS_TO_TICKS(1));
    return ESP_OK;
}

esp_err_t stcc4_set_rht_compensation(uint16_t rawTemperature, uint16_t rawHumidity)
{
    uint8_t buf[8] = {0xe0, 0x00};
    buf[2] = (rawTemperature >> 8) & 0xFF;
    buf[3] = rawTemperature & 0xFF;
    buf[4] = crcCheck(&buf[2], 2);
    buf[5] = (rawHumidity >> 8) & 0xFF;
    buf[6] = rawHumidity & 0xFF;
    buf[7] = crcCheck(&buf[5], 2);
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
    vTaskDelay(pdMS_TO_TICKS(1));
    return ESP_OK;
}

esp_err_t stcc4_measure_single_shot()
{
    uint8_t buf[2] = {0x21, 0x9D};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
    vTaskDelay(pdMS_TO_TICKS(500));
    return ESP_OK;
}

esp_err_t stcc4_enter_sleep_mode()
{
    uint8_t buf[2] = {0x36, 0x50};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 enter sleep mode");
#endif
    vTaskDelay(pdMS_TO_TICKS(2));
    return ESP_OK;
}

esp_err_t stcc4_exit_sleep_mode()
{
    uint8_t buf[2] = {0x00, 0x00};
    i2c_operation_job_t i2c_ops[] = {
        {.command = I2C_MASTER_CMD_START},
        {.command = I2C_MASTER_CMD_WRITE, .write = {.ack_check = false, .data = buf, .total_bytes = 2}},
        {.command = I2C_MASTER_CMD_STOP},
    };

    i2c_master_execute_defined_operations(stcc4_i2c_dev_handle, i2c_ops, sizeof(i2c_ops) / sizeof(i2c_operation_job_t), -1);
    // i2c_master_transmit(stcc4_i2c_dev_handle, buf, 2, -1);
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 exit sleep mode");
#endif
    vTaskDelay(pdMS_TO_TICKS(5));
    return ESP_OK;
}

esp_err_t stcc4_perform_conditioning()
{
    uint8_t buf[2] = {0x29, 0xBC};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 perform conditioning");
#endif
    vTaskDelay(pdMS_TO_TICKS(22000));
    return ESP_OK;
}

esp_err_t stcc4_perform_factory_reset(uint16_t *factoryResetResult)
{
    uint8_t buf[2] = {0x36, 0x32};
    uint8_t receive_buf[2] = {0};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
    vTaskDelay(pdMS_TO_TICKS(90));
    ESP_ERROR_CHECK(i2c_master_receive(stcc4_i2c_dev_handle, receive_buf, sizeof(receive_buf), -1));
    *factoryResetResult = (receive_buf[0] << 8) | receive_buf[1];
    return ESP_OK;
}

esp_err_t stcc4_perform_self_test(uint16_t *testResult)
{
    uint8_t buf[2] = {0x27, 0x8c};
    uint8_t receive_buf[3] = {0};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 performing self test");
#endif
    vTaskDelay(pdMS_TO_TICKS(360));
    ESP_ERROR_CHECK(i2c_master_receive(stcc4_i2c_dev_handle, receive_buf, sizeof(receive_buf), -1));
    if (crcCheck(receive_buf, 2) != receive_buf[2])
    {
        ESP_LOGE(LOG_TAG, "Self test CRC check failed");
        return ESP_ERR_INVALID_CRC;
    }
    *testResult = (receive_buf[0] << 8) | receive_buf[1];
    return ESP_OK;
}

esp_err_t stcc4_enable_testing_mode()
{
    uint8_t buf[2] = {0x3F, 0xBC};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 enable testing mode");
#endif
    return ESP_OK;
}

esp_err_t stcc4_disable_testing_mode()
{
    uint8_t buf[2] = {0x3F, 0x3D};
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
#if CONFIG_STCC4_DEBUG_OUTPUT
    ESP_LOGI(LOG_TAG, "STCC4 disable testing mode");
#endif
    return ESP_OK;
}

esp_err_t stcc4_perform_forced_recalibration(int16_t targetCO2Concentration, int16_t *frcCorrection)
{
    uint8_t buf[5] = {0x36, 0x2F};
    uint8_t receive_buf[3] = {0};
    buf[2] = (targetCO2Concentration >> 8) & 0xFF;
    buf[3] = targetCO2Concentration & 0xFF;
    buf[4] = crcCheck(&buf[2], 2);
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));

    vTaskDelay(pdMS_TO_TICKS(90));

    ESP_ERROR_CHECK(i2c_master_receive(stcc4_i2c_dev_handle, receive_buf, sizeof(receive_buf), -1));
    if (crcCheck(receive_buf, 2) != receive_buf[2])
    {
        ESP_LOGE(LOG_TAG, "perform Forced Recalibration CRC check failed");
        return ESP_ERR_INVALID_CRC;
    }
    *frcCorrection = (receive_buf[0] << 8) | receive_buf[1];
    return ESP_OK;
}

esp_err_t stcc4_get_product_id(uint32_t *productId, uint64_t *serialNumber)
{
    uint8_t buf[2] = {0x36, 0x5b};
    uint8_t receive_buf[18] = {0};
    uint32_t id = 0;
    uint64_t sn = 0;
    ESP_ERROR_CHECK(i2c_master_transmit(stcc4_i2c_dev_handle, buf, sizeof(buf), -1));
    vTaskDelay(pdMS_TO_TICKS(1));
    ESP_ERROR_CHECK(i2c_master_receive(stcc4_i2c_dev_handle, receive_buf, sizeof(receive_buf), -1));

    for (uint8_t i = 0; i < sizeof(receive_buf) / 3; i++)
    {
        if (crcCheck(&receive_buf[i * 3], 2) != receive_buf[i * 3 + 2])
        {
            ESP_LOGE(LOG_TAG, "getProductId CRC check failed, i= %d", i);
            return ESP_ERR_INVALID_CRC;
        }
    }
    id |= (uint32_t)receive_buf[0] << 24;
    id |= (uint32_t)receive_buf[1] << 16;
    id |= (uint32_t)receive_buf[3] << 8;
    id |= (uint32_t)receive_buf[4];
    *productId = id;

    sn |= (uint64_t)receive_buf[6] << 56;
    sn |= (uint64_t)receive_buf[7] << 48;
    sn |= (uint64_t)receive_buf[9] << 40;
    sn |= (uint64_t)receive_buf[10] << 32;
    sn |= (uint64_t)receive_buf[12] << 24;
    sn |= (uint64_t)receive_buf[13] << 16;
    sn |= (uint64_t)receive_buf[15] << 8;
    sn |= (uint64_t)receive_buf[16];
    *serialNumber = sn;
    return ESP_OK;
}
