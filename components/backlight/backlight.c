#include "backlight.h"
#include "esp_log.h"

#define TAG "backlight"
// 初始化PWM调光引脚
void lcd_backlight_init(void)
{
    ESP_LOGI(TAG, "Initializing LCD backlight PWM, Frequency: %dHz", CONFIG_LCD_BACKLIGHT_FREQUENCY);
    // 初始化LEDC 定时器
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = CONFIG_LCD_BACKLIGHT_FREQUENCY,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL_0,
        .duty = 0,
        .gpio_num = CONFIG_LCD_BACKLIGHT_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

// 设置PWM占空比
void lcd_backlight_set_duty(uint8_t duty)
{
    // 限制占空比
    if (duty >= 100)
        duty = 100;

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty * 81)); // 0 ~ 2^13=8192
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void lcd_backlight_get_duty(uint8_t *duty)
{
    *duty = ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0) / 81;
}