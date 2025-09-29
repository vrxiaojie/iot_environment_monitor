#pragma once
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl.h"
#include "esp_log.h"
#include <stdio.h>
#include "stcc4.h"
#include "sgp4x.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "time.h"

// 按1分钟、1小时、1天来存储数据点
typedef struct
{
    int32_t oneMinute[13];
    int32_t oneHour[13];
    int32_t oneDay[25];
} chart_data_points_t;

typedef struct
{
    chart_data_points_t co2;
    chart_data_points_t temperature;
    chart_data_points_t humidity;
    chart_data_points_t voc;
    int8_t oneMinute_cnt; // 1分钟数据计数
    int8_t oneHour_cnt;   // 1小时数据计数
    int8_t oneDay_cnt;    // 1天数据计数
} chart_data_t;

typedef enum
{
    CHART_TYPE_CO2 = 0,
    CHART_TYPE_TEMPERATURE,
    CHART_TYPE_HUMIDITY,
    CHART_TYPE_VOC,
} chart_type_t;

typedef enum
{
    TIME_FRAME_1MIN = 0,
    TIME_FRAME_1HOUR,
    TIME_FRAME_1DAY,
} time_frame_t;

extern chart_data_t chart_data;
extern time_frame_t current_time_frame;
extern chart_type_t current_chart_type;
extern TaskHandle_t update_chart_task_handle;
extern TaskHandle_t get_data_task_handle;

void update_chart_task(void *arg);
void get_data_task(void *arg);
void create_chart();
void delete_chart();
void update_chart_x_scale_text();
void set_x_tick_count();