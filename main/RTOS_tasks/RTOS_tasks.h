#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

extern TaskHandle_t bat_adc_task_handle;
extern TaskHandle_t stcc4_task_handle;
extern TaskHandle_t spg4x_task_handle;

void sgp4x_task(void *arg);
void stcc4_task(void *arg);
void status_bar_init_task(void *pvParameter);
void bat_adc_task(void *arg);
void get_data_task(void *arg);