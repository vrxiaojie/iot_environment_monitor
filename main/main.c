/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/lock.h>
#include <sys/param.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#include "driver/i2c_master.h"
// GT911触摸
#include "esp_lcd_touch_gt911.h"
#include "esp_lcd_touch.h"
// 传感器
#include "stcc4.h"
#include "sgp4x.h"
// LVGL UI
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl.h"
// LCD
#include "backlight.h"

#include "RTOS_tasks.h"

#include "wifi.h"
static const char *TAG = "example";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

#if CONFIG_USE_DOUBLE_FB
#define LCD_NUM_FB 2
#else
#define LCD_NUM_FB 1
#endif // CONFIG_USE_DOUBLE_FB

#define DATA_BUS_WIDTH 16
#define PIXEL_SIZE 2
#define LV_COLOR_FORMAT LV_COLOR_FORMAT_RGB565

// 是否启用GT911触摸，等屏幕能正常显示后开启
#define ENABLE_TOUCH_GT911 1
#define I2C_MASTER_NUM I2C_NUM_0
#if ENABLE_TOUCH_GT911

i2c_master_bus_handle_t bus_handle;

lv_ui guider_ui;
static void lvgl_touch_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    uint16_t touchpad_x[1] = {0};
    uint16_t touchpad_y[1] = {0};
    uint8_t touchpad_cnt = 0;

    esp_lcd_touch_handle_t touch_pad = lv_indev_get_user_data(indev);
    esp_lcd_touch_read_data(touch_pad);
    /* Get coordinates */
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(touch_pad, touchpad_x, touchpad_y, NULL, &touchpad_cnt, 1);

    if (touchpad_pressed && touchpad_cnt > 0)
    {
        data->point.x = touchpad_x[0];
        data->point.y = touchpad_y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

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
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your Application ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LVGL_DRAW_BUF_LINES 50 // number of display lines in each draw buffer
#define LVGL_TICK_PERIOD_MS 2
#define LVGL_TASK_STACK_SIZE (16 * 1024)
#define LVGL_TASK_PRIORITY 2
#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 1000 / CONFIG_FREERTOS_HZ

// LVGL library is not thread-safe, this example will call LVGL APIs from different tasks, so use a mutex to protect it
_lock_t lvgl_api_lock;

extern void lvgl_demo_ui(lv_display_t *disp);

static bool notify_lvgl_flush_ready(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_ctx)
{
    lv_display_t *disp = (lv_display_t *)user_ctx;
    lv_display_flush_ready(disp);
    return false;
}

static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // pass the draw buffer to the driver
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, px_map);
}

static void increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static void lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    while (1)
    {
        _lock_acquire(&lvgl_api_lock);
        lv_timer_handler();
        _lock_release(&lvgl_api_lock);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

esp_lcd_panel_handle_t panel_handle = NULL;
void app_main(void)
{
    // 初始化背光
    lcd_backlight_init();
    lcd_backlight_set_duty(0);

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

    // 初始化完毕 开启背光 设置亮度(默认20%)
    lcd_backlight_set_duty(20);
    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();
    // create a lvgl display
    lv_display_t *display = lv_display_create(LCD_H_RES, LCD_V_RES);
    // associate the rgb panel handle to the display
    lv_display_set_user_data(display, panel_handle);
    // set color depth
    lv_display_set_color_format(display, LV_COLOR_FORMAT);
    // create draw buffers
    void *buf1 = NULL;
    void *buf2 = NULL;

    // 分配LVGL绘图缓冲区 （这里使用双缓冲）
#if CONFIG_USE_DOUBLE_FB
    ESP_LOGI(TAG, "Use frame buffers as LVGL draw buffers");
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &buf1, &buf2));
    // set LVGL draw buffers and direct mode
    lv_display_set_buffers(display, buf1, buf2, LCD_H_RES * LCD_V_RES * PIXEL_SIZE, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    ESP_LOGI(TAG, "Allocate LVGL draw buffers");
    //  it's recommended to allocate the draw buffer from internal memory, for better performance
    size_t draw_buffer_sz = LCD_H_RES * LVGL_DRAW_BUF_LINES * PIXEL_SIZE;
    buf1 = heap_caps_malloc(draw_buffer_sz, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    assert(buf1);
    // set LVGL draw buffers and partial mode
    lv_display_set_buffers(display, buf1, buf2, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif // CONFIG_USE_DOUBLE_FB

    // set the callback which can copy the rendered image to an area of the display
    lv_display_set_flush_cb(display, lvgl_flush_cb);

    ESP_LOGI(TAG, "Register event callbacks");
    esp_lcd_rgb_panel_event_callbacks_t cbs = {
        .on_color_trans_done = notify_lvgl_flush_ready,
    };
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, display));

    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_lvgl_tick,
        .name = "lvgl_tick"};
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

    ESP_LOGI(TAG, "Create LVGL task");
    xTaskCreatePinnedToCore(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE,
                            NULL, LVGL_TASK_PRIORITY, NULL, 1);

    _lock_acquire(&lvgl_api_lock);
    events_init(&guider_ui);
    setup_ui(&guider_ui);
    _lock_release(&lvgl_api_lock);
    // 初始化GT911触摸
#if ENABLE_TOUCH_GT911
    // 初始化I2C

    ESP_ERROR_CHECK(i2c_master_init(&bus_handle));
    ESP_LOGI(TAG, "I2C initialized successfully");

    ESP_LOGI(TAG, "Initialize GT911 touch panel");
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    io_config.scl_speed_hz = CONFIG_I2C_MASTER_FREQUENCY;
    // 地址 0x5D (ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS) 或 0x14  (ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS_BACKUP)
    esp_lcd_touch_io_gt911_config_t tp_gt911_config = {
        .dev_addr = io_config.dev_addr,
    };
    // 配置触摸参数
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_H_RES,
        .y_max = LCD_V_RES,
        .rst_gpio_num = CONFIG_GT911_RST_GPIO,
        .int_gpio_num = CONFIG_GT911_INT_GPIO,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
        .driver_data = &tp_gt911_config,
    };

    // 将GT911挂载到I2C总线上
    esp_lcd_new_panel_io_i2c(bus_handle, &io_config, &tp_io_handle);

    // 为屏幕创建触摸设备
    esp_lcd_touch_handle_t tp = NULL;
    esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &tp);
    // 检查GT911是否初始化成功
    if (tp == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize GT911 touch panel");
        return;
    }
    else
    {
        ESP_LOGI(TAG, "GT911 initialized successfully");
    }
    // 初始化LVGL输入设备 参考示例 spi_lcd_touch
    ESP_LOGI(TAG, "Initialize input device for LVGL");
    static lv_indev_t *indev;
    indev = lv_indev_create(); // Input device driver (Touch)
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(indev, display);
    lv_indev_set_user_data(indev, tp);
    lv_indev_set_read_cb(indev, lvgl_touch_cb);
    ESP_LOGI(TAG, "Touch panel initialized successfully");
#endif
    xTaskCreate(sgp4x_task, "sgp4x_task", 4 * 1024, NULL, 5, NULL);
    xTaskCreate(stcc4_task, "stcc4_task", 4 * 1024, NULL, 5, NULL);
    // WiFi相关的初始化
    wifi_init();
}
