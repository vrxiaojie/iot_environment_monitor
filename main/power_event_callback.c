#include "power_management.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "AW32001"
extern TaskHandle_t update_battery_task;

TaskHandle_t aw32001_interrupt_task_handle = NULL;

void aw32001_isr_handler(void *arg)
{
    xTaskNotifyGive(aw32001_interrupt_task_handle);
}

void aw32001_interrupt_task(void *arg)
{
    aw32001_read_sys_status(&pwr_sys_status);
    ESP_LOGW(TAG, "aw32001_interrupt_task created");
    while (1)
    {
        // 等待中断事件发生
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        aw32001_read_sys_status(&pwr_sys_status);
        switch (pwr_sys_status.chg_stat)
        {
        // TODO: 通知状态栏充电状态更新任务、电源管理页面label更新任务
        case AW32001_CHG_STAT_NOT_CHARGING:
            ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = NOT CHARGING");
            break;
        case AW32001_CHG_STAT_PRE_CHARGE:
            ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = PRE CHARGE");
            break;
        case AW32001_CHG_STAT_FAST_CHARGE:
            ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = FAST CHARGE");
            break;
        case AW32001_CHG_STAT_CHARGE_DONE:
            ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = CHARGE DONE");
            break;
        }
        switch (pwr_sys_status.pg_stat)
        {
        case true:
            if (update_battery_task)
            {
                xTaskNotifyGive(update_battery_task);
            }

            ESP_LOGW(TAG, "AW32001 Interrupt: Power Source = USB POWER");
            break;
        case false:
            if (update_battery_task)
            {
                xTaskNotifyGive(update_battery_task);
            }
            ESP_LOGW(TAG, "AW32001 Interrupt: Power Source = BATTERY POWER");
            break;
        }
    }
}