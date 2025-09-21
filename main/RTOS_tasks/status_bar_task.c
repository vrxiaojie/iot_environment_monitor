#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sntp.h"
#include "time.h"
#include "status_bar.h"
#include "wifi.h"
#include "bat_adc.h"

void status_bar_update_task(void *pvParameter)
{
    (void)pvParameter;

    char time_buffer[24];
    struct tm timeinfo;
    time_t now;

    while (1)
    {
        // Update time
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d  %H:%M", &timeinfo);
        status_bar_set_time(time_buffer);

        // Update WiFi status
        status_bar_set_wifi_state(is_wifi_connected());

        // Update battery level
        // int battery_level = bat_adc_get_battery_level(); //TODO:实现电池电量接口
        int battery_level = 50;
        status_bar_set_battery_level(battery_level);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Update every second
    }
}
