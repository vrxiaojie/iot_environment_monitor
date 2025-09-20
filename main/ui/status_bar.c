#include "status_bar.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl.h"

static lv_obj_t *time_label;
static lv_obj_t *wifi_icon;
static lv_obj_t *battery_icon;
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

    // Time label on the left
    time_label = lv_label_create(status_bar);
    lv_obj_set_style_text_color(time_label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(time_label, "00:00");
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 5, 0);

    // Battery icon on the right
    battery_icon = lv_label_create(status_bar);
    lv_obj_set_style_text_color(battery_icon, lv_color_hex(0xffffff), 0);
    lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_FULL);
    lv_obj_align(battery_icon, LV_ALIGN_RIGHT_MID, -5, 0);

    // WiFi icon to the left of the battery icon
    wifi_icon = lv_label_create(status_bar);
    lv_obj_set_style_text_color(wifi_icon, lv_color_hex(0xffffff), 0);
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_align_to(wifi_icon, battery_icon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
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
            lv_label_set_text(wifi_icon, " "); // Or some disconnected icon
        }
        _lock_release(&lvgl_api_lock);
    }
}

void status_bar_set_battery_level(int level)
{
    if (battery_icon)
    {
        _lock_acquire(&lvgl_api_lock);
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
