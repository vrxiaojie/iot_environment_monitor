#pragma once

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

#define DEFAULT_OTA_URL "https://raw.gitcode.com/VRxiaojie/testOTA/raw/main/version.json"

typedef struct {
    uint8_t power_save_mode; // 省电模式
    uint8_t charge_limit;    // 电量百分比上限
} power_settings_t;


extern power_settings_t power_settings;

typedef struct
{
    char uri[64];
    uint32_t port;
    char username[64];
    char password[64];
    uint8_t upload_interval;
    uint8_t auto_conn;
} mqtt_user_config_t;

extern mqtt_user_config_t mqtt_user_config;

typedef struct
{
    char info_url[128];
    char newest_version[32];
    char download_url[256];
} ota_settings_t;

extern ota_settings_t ota_settings;

typedef struct {
    char api_key[64];
    char api_host[64];     // 和风天气API主机地址
    char city[32];         // 城市 支持拼音
} weather_config_t;
extern weather_config_t weather_config;

typedef enum
{
    NVS_READ_PWR,
    NVS_READ_MQTT,
    NVS_READ_OTA,
    NVS_READ_WEATHER
} nvs_read_idx_t;

typedef enum
{
    NVS_WRITE_PWR,
    NVS_WRITE_MQTT,
    NVS_WRITE_OTA,
    NVS_WRITE_WEATHER
} nvs_write_idx_t;

extern volatile nvs_read_idx_t nvs_read_idx;
extern volatile nvs_write_idx_t nvs_write_idx;

void nvs_init(void);
void nvs_read(nvs_read_idx_t idx);
void nvs_write(nvs_write_idx_t idx, void* arg);