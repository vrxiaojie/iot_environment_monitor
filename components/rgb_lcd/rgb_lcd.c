#include <stdio.h>
#include "rgb_lcd.h"

#define TAG "rgb_lcd"

esp_lcd_panel_handle_t panel_handle = NULL;
TaskHandle_t rgb_lcd_restart_panel_task_handle = NULL;

void rgb_lcd_set_pclk(uint32_t pclk_hz)
{
    if (panel_handle)
    {
        esp_lcd_rgb_panel_set_pclk(panel_handle, pclk_hz);
    }
}

static void rgb_lcd_restart_panel_task(void *args)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (panel_handle)
        {
            esp_lcd_rgb_panel_set_pclk(panel_handle, 10 * 1000 * 1000);
            // esp_lcd_rgb_panel_restart(panel_handle);
        }
    }
}

void rgb_lcd_init(void)
{
    ESP_LOGI(TAG, "Install RGB LCD panel driver");
    esp_lcd_rgb_panel_config_t panel_config = {
        .data_width = DATA_BUS_WIDTH,
        .dma_burst_size = 64,
        .num_fbs = LCD_NUM_FB,
#if CONFIG_USE_BOUNCE_BUFFER
        .bounce_buffer_size_px = 20 * LCD_H_RES,
#endif
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .disp_gpio_num = PIN_NUM_DISP_EN,
        .pclk_gpio_num = PIN_NUM_PCLK,
        .vsync_gpio_num = PIN_NUM_VSYNC,
        .hsync_gpio_num = PIN_NUM_HSYNC,
        .de_gpio_num = PIN_NUM_DE,
        .data_gpio_nums = {
            PIN_NUM_DATA0,
            PIN_NUM_DATA1,
            PIN_NUM_DATA2,
            PIN_NUM_DATA3,
            PIN_NUM_DATA4,
            PIN_NUM_DATA5,
            PIN_NUM_DATA6,
            PIN_NUM_DATA7,
            PIN_NUM_DATA8,
            PIN_NUM_DATA9,
            PIN_NUM_DATA10,
            PIN_NUM_DATA11,
            PIN_NUM_DATA12,
            PIN_NUM_DATA13,
            PIN_NUM_DATA14,
            PIN_NUM_DATA15,
        },
        .timings = {
            .pclk_hz = LCD_PIXEL_CLOCK_HZ,
            .h_res = LCD_H_RES,
            .v_res = LCD_V_RES,
            .hsync_back_porch = LCD_HBP,
            .hsync_front_porch = LCD_HFP,
            .hsync_pulse_width = LCD_HSYNC,
            .vsync_back_porch = LCD_VBP,
            .vsync_front_porch = LCD_VFP,
            .vsync_pulse_width = LCD_VSYNC,
            .flags = {
                .hsync_idle_low = 0,  // HSYNC 信号空闲时的电平，0：高电平，1：低电平
                .vsync_idle_low = 0,  // VSYNC 信号空闲时的电平，0 表示高电平，1：低电平
                .de_idle_high = 0,    // DE 信号空闲时的电平，0：低电平，1：高电平 (文档写反了?)
                .pclk_active_neg = 1, // 时钟信号的有效边沿，0：上升沿有效，1：下降沿有效
                .pclk_idle_high = 0,  // PCLK 信号空闲时的电平，0：低电平，1：高电平 (文档写反了?)
            },
        },
        .flags.fb_in_psram = true, // allocate frame buffer in PSRAM
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));

    ESP_LOGI(TAG, "Initialize RGB LCD panel");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    xTaskCreatePinnedToCoreWithCaps(rgb_lcd_restart_panel_task, "rgb_lcd_restart_panel_task", 2 * 1024, NULL, 3, &rgb_lcd_restart_panel_task_handle, 1, MALLOC_CAP_SPIRAM);
}
