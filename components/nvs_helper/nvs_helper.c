#include <stdio.h>
#include "nvs_helper.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "nvs_helper"

volatile nvs_read_idx_t nvs_read_idx;
volatile nvs_write_idx_t nvs_write_idx;

static void nvs_open_handle(const char *namespace, nvs_open_mode_t mode, nvs_handle_t *handle)
{
    esp_err_t err = nvs_open(namespace, mode, handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
    }
}

void nvs_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS分区有问题，尝试擦除并重新初始化
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    ESP_LOGI(TAG, "NVS initialized successfully");
}

power_settings_t power_settings = {};
static void nvs_read_power_settings(void)
{
    nvs_handle_t handle;
    esp_err_t err;
    nvs_open_handle("pwr_setting", NVS_READWRITE, &handle);
    err = nvs_get_u8(handle, "pwr_save_mode", &power_settings.power_save_mode);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        power_settings.power_save_mode = 0;
        nvs_set_u8(handle, "pwr_save_mode", power_settings.power_save_mode);
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    err = nvs_get_u8(handle, "charge_limit", &power_settings.charge_limit);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        power_settings.charge_limit = 70;
        nvs_set_u8(handle, "charge_limit", power_settings.charge_limit);
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    nvs_close(handle);
}

static void nvs_write_power_settings(power_settings_t new_power_settings)
{
    nvs_handle_t handle;
    nvs_open_handle("pwr_setting", NVS_READWRITE, &handle);
    nvs_read_power_settings();
    if (new_power_settings.power_save_mode != power_settings.power_save_mode)
    {
        ESP_ERROR_CHECK(nvs_set_u8(handle, "pwr_save_mode", new_power_settings.power_save_mode));
    }
    if (new_power_settings.charge_limit != power_settings.charge_limit)
    {
        ESP_ERROR_CHECK(nvs_set_u8(handle, "charge_limit", new_power_settings.charge_limit));
    }
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
}

mqtt_user_config_t mqtt_user_config = {};
static void mqtt_read_settings()
{
    nvs_handle_t handle;
    esp_err_t err;
    nvs_open_handle("mqtt_setting", NVS_READWRITE, &handle);
    size_t required_size;

    err = nvs_get_str(handle, "uri", NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_str(handle, "uri", "mqtt://your-broker");
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    else if (err == ESP_OK)
    {
        char* uri = malloc(required_size);
        memcpy(uri, mqtt_user_config.uri, required_size);
        err = nvs_get_str(handle, "uri", mqtt_user_config.uri, &required_size);
        free((void*)uri);
    }

    err = nvs_get_str(handle, "password", NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_str(handle, "password", "passwd");
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    else if (err == ESP_OK)
    {
        char* passwd = malloc(required_size);
        memcpy(passwd, mqtt_user_config.password, required_size);
        err = nvs_get_str(handle, "password", mqtt_user_config.password, &required_size);
        free((void*)passwd);
    }

    err = nvs_get_str(handle, "username", NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_str(handle, "username", "username");
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    else if (err == ESP_OK)
    {
        char* username = malloc(required_size);
        memcpy(username, mqtt_user_config.username, required_size);
        err = nvs_get_str(handle, "username", mqtt_user_config.username, &required_size);
        free((void*)username);
    }

    err = nvs_get_u32(handle, "port", &mqtt_user_config.port);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_u32(handle, "port", 1883);
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    err = nvs_get_u8(handle, "upload_intv", &mqtt_user_config.upload_interval);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_u8(handle, "upload_intv", 5);
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    err = nvs_get_u8(handle, "auto_conn", &mqtt_user_config.auto_conn);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_u8(handle, "auto_conn", 0);
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    nvs_close(handle);
}

static void mqtt_write_settings(mqtt_user_config_t new_mqtt_user_config)
{
    nvs_handle_t handle;
    nvs_open_handle("mqtt_setting", NVS_READWRITE, &handle);
    mqtt_read_settings();

    if (strcmp(new_mqtt_user_config.uri, mqtt_user_config.uri) != 0)
    {
        ESP_ERROR_CHECK(nvs_set_str(handle, "uri", new_mqtt_user_config.uri));
    }
    if (strcmp(new_mqtt_user_config.password, mqtt_user_config.password) != 0)
    {
        ESP_ERROR_CHECK(nvs_set_str(handle, "password", new_mqtt_user_config.password));
    }
    if (strcmp(new_mqtt_user_config.username, mqtt_user_config.username) != 0)
    {
        ESP_ERROR_CHECK(nvs_set_str(handle, "username", new_mqtt_user_config.username));
    }
    if (new_mqtt_user_config.port != mqtt_user_config.port)
    {
        ESP_ERROR_CHECK(nvs_set_u32(handle, "port", new_mqtt_user_config.port));
    }
    if (new_mqtt_user_config.upload_interval != mqtt_user_config.upload_interval)
    {
        ESP_ERROR_CHECK(nvs_set_u8(handle, "upload_intv", new_mqtt_user_config.upload_interval));
    }
    if (new_mqtt_user_config.auto_conn != mqtt_user_config.auto_conn)
    {
        ESP_ERROR_CHECK(nvs_set_u8(handle, "auto_conn", new_mqtt_user_config.auto_conn));
    }
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
}

ota_settings_t ota_settings = {.download_url = DEFAULT_OTA_URL};
static void ota_read_settings()
{
    nvs_handle_t handle;
    esp_err_t err;
    nvs_open_handle("ota_setting", NVS_READWRITE, &handle);
    size_t required_size;

    err = nvs_get_str(handle, "url", NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_str(handle, "url", DEFAULT_OTA_URL);
        ESP_ERROR_CHECK(nvs_commit(handle));
        strcpy(ota_settings.info_url, DEFAULT_OTA_URL);
    }
    else if (err == ESP_OK)
    {
        char* info_url = malloc(required_size);
        memcpy(info_url, ota_settings.info_url, required_size);
        err = nvs_get_str(handle, "url", ota_settings.info_url, &required_size);
        free((void*)info_url);
    }
    nvs_close(handle);
}

static void ota_write_settings(ota_settings_t new_ota_settings)
{
    nvs_handle_t handle;
    nvs_open_handle("ota_setting", NVS_READWRITE, &handle);
    ota_read_settings();

    if (strcmp(new_ota_settings.info_url, ota_settings.info_url) != 0)
    {
        ESP_ERROR_CHECK(nvs_set_str(handle, "url", new_ota_settings.info_url));
    }
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
}

static TaskHandle_t nvs_write_task_handle = NULL;
static void nvs_write_task(void *arg)
{
    switch (nvs_write_idx)
    {
    case NVS_WRITE_PWR:
        nvs_write_power_settings(power_settings);
        break;
    case NVS_WRITE_MQTT:
        mqtt_write_settings(mqtt_user_config);
        break;
    case NVS_WRITE_OTA:
        ota_write_settings(ota_settings);
        break;
    }
    nvs_write_task_handle = NULL;
    vTaskDelete(NULL);
}

static TaskHandle_t nvs_read_task_handle = NULL;
static void nvs_read_task(void *arg)
{
    switch (nvs_read_idx)
    {
    case NVS_READ_PWR:
        nvs_read_power_settings();
        break;
    case NVS_READ_MQTT:
        mqtt_read_settings();
        break;
    case NVS_READ_OTA:
        ota_read_settings();
        break;
    }
    nvs_read_task_handle = NULL;
    vTaskDelete(NULL);
}

void nvs_read(nvs_read_idx_t idx)
{
    nvs_read_idx = idx;
    if (nvs_read_task_handle != NULL)
    {
        vTaskDelete(nvs_read_task_handle);
        nvs_read_task_handle = NULL;
    }
    xTaskCreate(nvs_read_task, "nvs_read_task", 8 * 1024, NULL, 10, &nvs_read_task_handle);
}

void nvs_write(nvs_write_idx_t idx, void *arg)
{
    switch (idx)
    {
    case NVS_WRITE_PWR:
        if (arg != NULL)
        {
            // power_settings_t *new_power_settings = (power_settings_t *)arg;
            power_settings = *(power_settings_t *)arg;
        }
        break;
    case NVS_WRITE_MQTT:
        if (arg != NULL)
        {
            // mqtt_user_config_t *new_mqtt_user_config = (mqtt_user_config_t *)arg;
            mqtt_user_config = *(mqtt_user_config_t *)arg;
        }
        break;
    case NVS_WRITE_OTA:
        if (arg != NULL)
        {
            // ota_settings_t *new_ota_settings = (ota_settings_t *)arg;
            ota_settings = *(ota_settings_t *)arg;
        }
        break;
    }
    nvs_write_idx = idx;
    if (nvs_write_task_handle != NULL)
    {
        vTaskDelete(nvs_write_task_handle);
        nvs_write_task_handle = NULL;
    }
    xTaskCreate(nvs_write_task, "nvs_write_task", 8 * 1024, NULL, 10, &nvs_write_task_handle);
}