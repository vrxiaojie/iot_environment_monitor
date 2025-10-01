#include "lvgl_setup.h"

#define TAG "lvgl_setup"

#define PIXEL_SIZE 2
#define LV_COLOR_FORMAT LV_COLOR_FORMAT_RGB565
#define LVGL_DRAW_BUF_LINES 50 // number of display lines in each draw buffer
#define LVGL_TICK_PERIOD_MS 1
#define LVGL_TASK_STACK_SIZE (16 * 1024)
#define LVGL_TASK_PRIORITY 2

// LVGL library is not thread-safe, this example will call LVGL APIs from different tasks, so use a mutex to protect it
_lock_t lvgl_api_lock;

lv_display_t *display = NULL;
extern i2c_master_bus_handle_t bus_handle;
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

void lvgl_init(void)
{
    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();
    // create a lvgl display
    display = lv_display_create(LCD_H_RES, LCD_V_RES);
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
    lv_display_set_buffers(display, buf1, buf2, LCD_H_RES * LCD_V_RES * PIXEL_SIZE, LV_DISPLAY_RENDER_MODE_FULL);
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
    xTaskCreatePinnedToCoreWithCaps(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE,
                            NULL, LVGL_TASK_PRIORITY, NULL, 1, MALLOC_CAP_SPIRAM);

    _lock_acquire(&lvgl_api_lock);
    events_init(&guider_ui);
    setup_ui(&guider_ui);
    _lock_release(&lvgl_api_lock);
}

void lvgl_touch_init()
{
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
}