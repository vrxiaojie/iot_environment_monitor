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
}