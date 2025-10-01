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
    while (1)
    {
        // 等待中断事件发生
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        aw32001_read_sys_status(&pwr_sys_status);
        switch (pwr_sys_status.chg_stat)
        {
        case AW32001_CHG_STAT_NOT_CHARGING:
            // ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = NOT CHARGING");
            break;
        case AW32001_CHG_STAT_PRE_CHARGE:
            // ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = PRE CHARGE");
            break;
        case AW32001_CHG_STAT_FAST_CHARGE:
            // ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = FAST CHARGE");
            break;
        case AW32001_CHG_STAT_CHARGE_DONE:
            // ESP_LOGW(TAG, "AW32001 Interrupt: Charge Status = CHARGE DONE");
            break;
        }
        switch (pwr_sys_status.pg_stat)
        {
        case true:
            if (update_battery_task)
            {
                xTaskNotifyGive(update_battery_task);
            }

            // ESP_LOGW(TAG, "AW32001 Interrupt: Power Source = USB POWER");
            break;
        case false:
            if (update_battery_task)
            {
                xTaskNotifyGive(update_battery_task);
            }
            // ESP_LOGW(TAG, "AW32001 Interrupt: Power Source = BATTERY POWER");
            break;
        }
    }
}

void aw32001_interrupt_init()
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_NEGEDGE; // 下降沿产生中断
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    io_conf.pin_bit_mask = 1ULL << 5;
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_5, aw32001_isr_handler, NULL);
    if (aw32001_interrupt_task_handle != NULL)
    {
        ESP_LOGW(TAG, "AW32001 interrupt task already created");
        return;
    }
    xTaskCreateWithCaps(aw32001_interrupt_task, "aw32001_interrupt_task", 8 * 1024, NULL, 6, &aw32001_interrupt_task_handle, MALLOC_CAP_SPIRAM);
}