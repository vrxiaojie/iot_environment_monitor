#pragma once
#include "rgb_lcd.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl.h"
#include "esp_lcd_touch_gt911.h"
#include "esp_lcd_touch.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "esp_timer.h"
#include <sys/lock.h>

extern lv_display_t *display;

void lvgl_init(void);
void lvgl_touch_init(void);
