#pragma once

#include "esp_err.h"
#include "esp_log.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 根据屏幕厂商的datasheet来设定以下参数
// 最高刷新率 = 10000000/(1+43+8+480)/(10+12+4+272)=63Hz
#define LCD_PIXEL_CLOCK_HZ (10 * 1000 * 1000) // 10MHz, 屏幕厂商给的是5~12
#define LCD_H_RES 480                         // 横向分辨率 480
#define LCD_V_RES 272                         // 纵向分辨率 272
#define LCD_HSYNC 1  // 水平同步脉冲宽度
#define LCD_HBP 43   // 水平后肩
#define LCD_HFP 8    // 水平前肩
#define LCD_VSYNC 10 // 垂直同步脉冲宽度
#define LCD_VBP 12   // 垂直后肩
#define LCD_VFP 4    // 垂直前肩

// IO设置
#define PIN_NUM_DISP_EN -1 // 未使用DISP引脚，默认上拉3.3V

#define PIN_NUM_HSYNC CONFIG_LCD_HSYNC_GPIO
#define PIN_NUM_VSYNC CONFIG_LCD_VSYNC_GPIO
#define PIN_NUM_DE CONFIG_LCD_DE_GPIO
#define PIN_NUM_PCLK CONFIG_LCD_PCLK_GPIO

#define PIN_NUM_DATA0 CONFIG_LCD_DATA0_GPIO
#define PIN_NUM_DATA1 CONFIG_LCD_DATA1_GPIO
#define PIN_NUM_DATA2 CONFIG_LCD_DATA2_GPIO
#define PIN_NUM_DATA3 CONFIG_LCD_DATA3_GPIO
#define PIN_NUM_DATA4 CONFIG_LCD_DATA4_GPIO
#define PIN_NUM_DATA5 CONFIG_LCD_DATA5_GPIO
#define PIN_NUM_DATA6 CONFIG_LCD_DATA6_GPIO
#define PIN_NUM_DATA7 CONFIG_LCD_DATA7_GPIO
#define PIN_NUM_DATA8 CONFIG_LCD_DATA8_GPIO
#define PIN_NUM_DATA9 CONFIG_LCD_DATA9_GPIO
#define PIN_NUM_DATA10 CONFIG_LCD_DATA10_GPIO
#define PIN_NUM_DATA11 CONFIG_LCD_DATA11_GPIO
#define PIN_NUM_DATA12 CONFIG_LCD_DATA12_GPIO
#define PIN_NUM_DATA13 CONFIG_LCD_DATA13_GPIO
#define PIN_NUM_DATA14 CONFIG_LCD_DATA14_GPIO
#define PIN_NUM_DATA15 CONFIG_LCD_DATA15_GPIO

// 设置Frame Buffer数量
#if CONFIG_USE_DOUBLE_FB
#define LCD_NUM_FB 2
#else
#define LCD_NUM_FB 1
#endif // CONFIG_USE_DOUBLE_FB

#define DATA_BUS_WIDTH 16

extern esp_lcd_panel_handle_t panel_handle;
extern TaskHandle_t rgb_lcd_restart_panel_task_handle;

void rgb_lcd_init(void);
void rgb_lcd_set_pclk(uint32_t pclk_hz);

