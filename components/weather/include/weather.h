#pragma once

#include "esp_http_client.h"

// 位置信息字段
typedef struct {
    char *province;        // 省份
    char *city;            // 城市
} location_info_t;

// 天气信息结构体
typedef struct {
    char* weather;          // 天气现象
    float temperature;      // 温度
    float feels_like;       // 体感温度
    float humidity;         // 湿度
    char* wind_dir;         // 风向
    char* wind_scale;       // 风力等级
    float wind_speed;       // 风速(km/h)
    float precip;           // 降水量(mm)
    float pressure;         // 气压(hPa)
    float visibility;       // 能见度(km)
    float cloud;            // 云量(%)
    float dew_point;       // 露点温度
    char* update_time;     // 更新时间
    location_info_t* location_info; // 位置信息
} weather_info_t;

typedef struct {
    char *api_key;
    char *api_host;     // 和风天气API主机地址
    char *city;         // 城市 支持拼音
} weather_config_t;

/**
 * @brief 获取天气信息
 * 
 * @param config    配置信息
 * @return weather_info_t*
 */
weather_info_t* weather_get(weather_config_t *config);

/**
 * @brief 打印天气信息
 * 
 * @param info  天气信息 weather_info_t*
 */
void weather_print_info(const weather_info_t *info);

/**
 * @brief 释放位置信息内存
 * 
 * @param location_info  位置信息 location_info_t*
 */
void location_info_free(location_info_t *location_info);

/**
 * @brief 释放天气信息内存
 * 
 * @param info  天气信息 weather_info_t*
 */
void weather_info_free(weather_info_t *info);

