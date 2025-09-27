#pragma once

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

typedef struct {
    uint8_t power_save_mode; // 省电模式
    uint8_t charge_limit;    // 电量百分比上限
} power_settings_t;


extern power_settings_t power_settings;

void nvs_init(void);
void nvs_read_power_settings(void);
void nvs_write_power_settings(power_settings_t new_power_settings);