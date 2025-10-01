#include "RTOS_tasks.h"
#include "bat_adc.h"
#include "nvs_helper.h"
#include "power_management.h"

float bat_level = 0.0f;
static bool fully_charged = false;
extern TaskHandle_t update_battery_task;
TaskHandle_t bat_adc_task_handle = NULL;
void bat_adc_task(void *arg)
{
    bat_adc_init();
    vTaskDelay(pdMS_TO_TICKS(50));
    while (1)
    {
        bat_adc_get_battery_level(&bat_level);
        if (bat_level > power_settings.charge_limit && !fully_charged)
        {
            fully_charged = true;
            aw32001_disable_charge();
        }
        else if (bat_level < power_settings.charge_limit - 2 && fully_charged)
        {
            fully_charged = false;
            aw32001_enable_charge();
        }
        else if (!fully_charged)
        {
            aw32001_enable_charge();
        }
        if (update_battery_task != NULL)
        {
            xTaskNotifyGive(update_battery_task);
        }
        ESP_LOGI("bat_adc_task", "Battery Level: %.1f%%", bat_level);
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(30000));
    }
}
