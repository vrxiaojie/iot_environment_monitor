#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

// 自定义组件
// 传感器
#include "stcc4.h"
#include "sgp4x.h"
// 背光+RGB LCD驱动
#include "backlight.h"
#include "rgb_lcd.h"
#include "wifi.h"
#include "status_bar.h"
#include "ntp.h"
#include "power_management.h"

#include "RTOS_tasks.h"
#include "lvgl_setup.h"

static const char *TAG = "main";

#define I2C_MASTER_NUM I2C_NUM_0
i2c_master_bus_handle_t bus_handle;
static esp_err_t i2c_master_init(i2c_master_bus_handle_t *bus_handle)
{

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = CONFIG_I2C_MASTER_SDA,
        .scl_io_num = CONFIG_I2C_MASTER_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));
    return ESP_OK;
}

void app_main(void)
{
    // 初始化背光
    lcd_backlight_init();
    lcd_backlight_set_duty(0);
    // 初始化I2C
    ESP_ERROR_CHECK(i2c_master_init(&bus_handle));
    ESP_LOGI(TAG, "I2C initialized successfully");
    // 初始化RGB LCD
    rgb_lcd_init();

    // 初始化完毕 开启背光 设置亮度(默认20%)
    lcd_backlight_set_duty(20);
    // 初始化LVGL
    lvgl_init();
    // 初始化GT911触摸
    lvgl_touch_init();

    // 初始化电源管理芯片
    aw32001_init(bus_handle);
    aw32001_disable_watchdog();
    aw32001_enable_charge();
    aw32001_interrupt_init();

    xTaskCreate(sgp4x_task, "sgp4x_task", 4 * 1024, NULL, 5, NULL);
    xTaskCreate(stcc4_task, "stcc4_task", 4 * 1024, NULL, 5, NULL);
    // WiFi相关的初始化
    wifi_init();
    wifi_event_init();
    xTaskCreate(ntp_sync_task, "ntp_sync_task", 4 * 1024, NULL, 4, NULL);
    xTaskCreate(status_bar_init_task, "status_bar_task", 2 * 1024, NULL, 5, NULL);
    xTaskCreate(bat_adc_task, "bat_adc_task", 4 * 1024, NULL, 3, NULL);
}
