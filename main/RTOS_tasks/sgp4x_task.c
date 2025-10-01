#include "RTOS_tasks.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sgp4x.h"
#include <sensirion_gas_index_algorithm.h>

#define TAG "sensor_sgp40"
int32_t voc_index = 0;
TaskHandle_t spg4x_task_handle = NULL;
extern i2c_master_bus_handle_t bus_handle;
void sgp4x_task(void *arg)
{
    sgp4x_config_t sgp4x_cfg = I2C_SGP40_CONFIG_DEFAULT;
    sgp4x_handle_t sgp4x_handle = NULL;

    // 初始化VOC指数算法
    GasIndexAlgorithmParams voc_params;
    GasIndexAlgorithm_init(&voc_params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);

    ESP_ERROR_CHECK(sgp4x_init(bus_handle, &sgp4x_cfg, &sgp4x_handle));
    while (1)
    {
        // 读取数据
        uint16_t sraw_voc;

        esp_err_t result = sgp40_measure_signals(sgp4x_handle, &sraw_voc);
        if (result != ESP_OK)
        {
            ESP_LOGE(TAG, "sgp4x device conditioning failed (%s)", esp_err_to_name(result));
        }
        else
        {
            GasIndexAlgorithm_process(&voc_params, sraw_voc, &voc_index);

            ESP_LOGI(TAG, "SRAW VOC: %u | VOC Index: %li", sraw_voc, voc_index);
        }
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}
