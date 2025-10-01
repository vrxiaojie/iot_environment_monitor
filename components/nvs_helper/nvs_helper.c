#include <stdio.h>
#include "nvs_helper.h"

#define TAG "nvs_helper"

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
void nvs_read_power_settings(void)
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

void nvs_write_power_settings(power_settings_t new_power_settings)
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
void mqtt_read_settings()
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
        mqtt_user_config.uri = malloc(required_size);
        err = nvs_get_str(handle, "uri", (char *)mqtt_user_config.uri, &required_size);
    }
    
    err = nvs_get_str(handle, "password", NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_str(handle, "password", "passwd");
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    else if (err == ESP_OK)
    {
        mqtt_user_config.password = malloc(required_size);
        err = nvs_get_str(handle, "password", (char *)mqtt_user_config.password, &required_size);
    }

    err = nvs_get_str(handle, "username", NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_set_str(handle, "username", "username");
        ESP_ERROR_CHECK(nvs_commit(handle));
    }
    else if (err == ESP_OK)
    {
        mqtt_user_config.username = malloc(required_size);
        err = nvs_get_str(handle, "username", (char *)mqtt_user_config.username, &required_size);
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

void mqtt_write_settings(mqtt_user_config_t new_mqtt_user_config)
{
    nvs_handle_t handle;
    nvs_open_handle("mqtt_setting", NVS_READWRITE, &handle);
    mqtt_read_settings();

    if (new_mqtt_user_config.uri != NULL && strcmp(new_mqtt_user_config.uri, mqtt_user_config.uri) != 0)
    {
        ESP_ERROR_CHECK(nvs_set_str(handle, "uri", new_mqtt_user_config.uri));
    }
    if (new_mqtt_user_config.password != NULL && strcmp(new_mqtt_user_config.password, mqtt_user_config.password) != 0)
    {
        ESP_ERROR_CHECK(nvs_set_str(handle, "password", new_mqtt_user_config.password));
    }
    if (new_mqtt_user_config.username != NULL && strcmp(new_mqtt_user_config.username, mqtt_user_config.username) != 0)
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