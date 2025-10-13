#include <stdio.h>
#include "lpm.h"
#include "driver/gpio.h"
#include "esp_pm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "backlight.h"

#define TIMEOUT_SECONDS (300) // 5分钟无操作后进入低功耗模式

static volatile uint8_t lpm_status = 0; // 0: 正常运行, 1: 进入低功耗模式
static volatile uint8_t is_touched = 0;
static TaskHandle_t lpm_task_handle = NULL;

static void touch_isr_handler(void *arg)
{
    BaseType_t *pxHigherPriorityTaskWoken = NULL;
    if (lpm_status == 1 && lpm_task_handle != NULL)
    {
        vTaskNotifyGiveFromISR(lpm_task_handle, pxHigherPriorityTaskWoken); // 唤醒lpm_task任务
    }
    if (lpm_status == 0)
    {
        is_touched = 1;
    }
}

static void lpm_task(void *arg)
{
    int16_t count = 0;
    while (1)
    {
        if (is_touched == 1 && lpm_status == 0)
        {
            is_touched = 0;
            count = 0;
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        if (count < TIMEOUT_SECONDS) // 进入低功耗模式
        {
            count++;
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        if (count >= TIMEOUT_SECONDS && lpm_status == 0)
        {
            // 进入低功耗模式
            gpio_isr_handler_remove(GPIO_NUM_41);
            lpm_status = 1;
            count = 0;
            // 关闭背光
            lcd_backlight_set_duty(0);
            esp_pm_config_t pm_config = {
                .max_freq_mhz = 80,
                .min_freq_mhz = 80,
                .light_sleep_enable = false,
            };
            ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
            gpio_isr_handler_add(GPIO_NUM_41, touch_isr_handler, NULL);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // 等待被中断唤醒
            // 恢复正常运行
            lpm_status = 0;
            pm_config.max_freq_mhz = 240;
            ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
            // 恢复背光
            // TODO: 亮度值设为全局变量
            lcd_backlight_set_duty(20);
        }
    }
}

void lpm_init(void)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_NEGEDGE; // 下降沿产生中断
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    io_conf.pin_bit_mask = 1ULL << 41;
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
}

void lpm_enable(void)
{
    gpio_isr_handler_add(GPIO_NUM_41, touch_isr_handler, NULL);
    if (lpm_task_handle == NULL)
    {
        xTaskCreateWithCaps(lpm_task, "lpm_task", 4 * 1024, NULL, 6, &lpm_task_handle, MALLOC_CAP_SPIRAM);
    }
}

void lpm_disable(void)
{
    if (lpm_task_handle != NULL)
    {
        vTaskDelete(lpm_task_handle);
        lpm_task_handle = NULL;
    }
    gpio_isr_handler_remove(GPIO_NUM_41);
    lpm_status = 0;
}