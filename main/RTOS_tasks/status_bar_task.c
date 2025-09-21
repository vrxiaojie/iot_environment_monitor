#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sntp.h"
#include "time.h"
#include "status_bar.h"
#include "wifi.h"
#include "bat_adc.h"
#include "esp_log.h"

extern float bat_level;
TaskHandle_t update_battery_task = NULL;
static void status_bar_update_battery_task(void *args)
{
    status_bar_set_battery_level(bat_level);
    while (1)
    {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == 1)
        {
            status_bar_set_battery_level(bat_level);
        }
    }
}

static void status_bar_update_time_task(void *args)
{
    char time_buffer[24], last_minute[3], now_minute[3];
    struct tm timeinfo;
    time_t now;
    time(&now);

    localtime_r(&now, &timeinfo);
    strftime(last_minute, sizeof(last_minute), "%M", &timeinfo);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d  %H:%M", &timeinfo);
    status_bar_set_time(time_buffer);
    while (1)
    {
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(now_minute, sizeof(now_minute), "%M", &timeinfo);
        // 每分钟更新一次时间显示
        if (strcmp(last_minute, now_minute) != 0)
        {
            // 当分钟变化时更新
            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d  %H:%M", &timeinfo);
            status_bar_set_time(time_buffer);
            strcpy(last_minute, now_minute);
        }
        if (sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED)
        {
            // 当系统时间同步成功时更新
            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d  %H:%M", &timeinfo);
            status_bar_set_time(time_buffer);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

TaskHandle_t update_wifi_icon_task_handle = NULL;
static void status_bar_update_wifi_icon_task(void *args)
{
    while (1)
    {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == 1)
        {
            status_bar_set_wifi_state(is_wifi_connected());
        }
    }
}

void status_bar_init_task(void *pvParameter)
{
    status_bar_create();
    xTaskCreate(status_bar_update_battery_task, "update_battery_task", 2 * 1024, NULL, 6, &update_battery_task);
    xTaskCreate(status_bar_update_time_task, "update_time_task", 4 * 1024, NULL, 6, NULL);
    xTaskCreate(status_bar_update_wifi_icon_task, "update_wifi_icon_task", 2 * 1024, NULL, 6, &update_wifi_icon_task_handle);
    vTaskDelete(NULL);
}
