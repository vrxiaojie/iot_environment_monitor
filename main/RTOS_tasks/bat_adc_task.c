#include "RTOS_tasks.h"
#include "bat_adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

float bat_level = 0.0f;
void bat_adc_task(void *arg)
{
    bat_adc_init();
    vTaskDelay(pdMS_TO_TICKS(50));
    while (1)
    {
        bat_adc_get_battery_level(&bat_level);
        ESP_LOGI("bat_adc_task", "Battery Level: %.1f%%", bat_level);
        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}
