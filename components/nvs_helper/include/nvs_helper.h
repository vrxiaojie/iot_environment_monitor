#pragma once

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

typedef struct {
    uint8_t power_save_mode; // 省电模式
    uint8_t charge_limit;    // 电量百分比上限
} power_settings_t;


extern power_settings_t power_settings;

typedef struct
{
    const char *uri;
    uint32_t port;
    const char *username;
    const char *password;
    uint8_t upload_interval;
    uint8_t auto_conn;
} mqtt_user_config_t;

extern mqtt_user_config_t mqtt_user_config;

typedef enum
{
    NVS_READ_PWR,
    NVS_READ_MQTT,
} nvs_read_idx_t;

typedef enum
{
    NVS_WRITE_PWR,
    NVS_WRITE_MQTT,
} nvs_write_idx_t;

extern volatile nvs_read_idx_t nvs_read_idx;
extern volatile nvs_write_idx_t nvs_write_idx;

void nvs_init(void);
void nvs_read(nvs_read_idx_t idx);
void nvs_write(nvs_write_idx_t idx, void* arg);