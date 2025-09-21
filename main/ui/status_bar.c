#include "status_bar.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl.h"
#include <stdio.h>

static lv_obj_t *time_label;
static lv_obj_t *wifi_icon;
static lv_obj_t *battery_icon;
static lv_obj_t *battery_level_label;
extern _lock_t lvgl_api_lock;
void status_bar_create()
{
    _lock_acquire(&lvgl_api_lock);
    lv_obj_t *status_bar = lv_obj_create(lv_layer_sys());
    lv_obj_set_size(status_bar, 480, 24);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(status_bar, 0, 0);
    lv_obj_set_style_radius(status_bar, 0, 0);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_scrollbar_mode(status_bar, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_SCROLLABLE);

    // 时间
    time_label = lv_label_create(status_bar);
    lv_obj_set_style_text_color(time_label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(time_label, "00:00");
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 5, 0);

    // 电池电量
    battery_icon = lv_label_create(status_bar);
    lv_obj_set_style_text_color(battery_icon, lv_color_hex(0xffffff), 0);
    lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_FULL);
    lv_obj_align(battery_icon, LV_ALIGN_RIGHT_MID, -5, 0);
    battery_level_label = lv_label_create(status_bar);
    lv_obj_set_style_text_color(battery_level_label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(battery_level_label, "100%%");
    lv_obj_set_align(battery_level_label, LV_TEXT_ALIGN_RIGHT);
    lv_obj_align_to(battery_level_label, battery_icon, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    // WIFI图标
    wifi_icon = lv_label_create(status_bar);
    lv_obj_set_style_text_color(wifi_icon, lv_color_hex(0xffffff), 0);
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_align_to(wifi_icon, battery_level_label, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    _lock_release(&lvgl_api_lock);
}

void status_bar_set_time(const char *time_str)
{
    if (time_label)
    {
        _lock_acquire(&lvgl_api_lock);
        lv_label_set_text(time_label, time_str);
        _lock_release(&lvgl_api_lock);
    }
}

void status_bar_set_wifi_state(bool connected)
{
    if (wifi_icon)
    {
        _lock_acquire(&lvgl_api_lock);
        if (connected)
        {
            lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
        }
        else
        {
            lv_label_set_text(wifi_icon, " ");
        }
        _lock_release(&lvgl_api_lock);
    }
}

void status_bar_set_battery_level(int level)
{
    if (battery_icon && battery_level_label)
    {
        _lock_acquire(&lvgl_api_lock);
        char buf[4];
        snprintf(buf, sizeof(buf), "%d%%", level);
        lv_label_set_text(battery_level_label, buf);
        if (level > 90)
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_FULL);
        }
        else if (level > 60)
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_3);
        }
        else if (level > 40)
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_2);
        }
        else if (level > 10)
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_1);
        }
        else
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_EMPTY);
        }
        _lock_release(&lvgl_api_lock);
    }
}
