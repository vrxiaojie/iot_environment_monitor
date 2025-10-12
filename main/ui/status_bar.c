#include "status_bar.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl.h"
#include "esp_log.h"
#include <stdio.h>

static lv_obj_t *time_label;
static lv_obj_t *wifi_icon;
static lv_obj_t *battery_icon;
static lv_obj_t *battery_level_label;
static lv_obj_t *charge_status_icon;
lv_obj_t *status_bar;
extern _lock_t lvgl_api_lock;

void status_bar_show(bool need_api_lock)
{
    if (status_bar && need_api_lock == false)
    {
        lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_HIDDEN);
    }
    else if (status_bar && need_api_lock == true)
    {
        _lock_acquire(&lvgl_api_lock);
        lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_HIDDEN);
        _lock_release(&lvgl_api_lock);
    }
}

void status_bar_hide(bool need_api_lock)
{
    if (status_bar && need_api_lock == false)
    {
        lv_obj_add_flag(status_bar, LV_OBJ_FLAG_HIDDEN);
    }
    else if (status_bar && need_api_lock == true)
    {
        _lock_acquire(&lvgl_api_lock);
        lv_obj_add_flag(status_bar, LV_OBJ_FLAG_HIDDEN);
        _lock_release(&lvgl_api_lock);
    }
}

void status_bar_create()
{
    _lock_acquire(&lvgl_api_lock);
    status_bar = lv_obj_create(lv_layer_sys());
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
    lv_obj_set_style_text_font(time_label, &lv_font_siyuanheiti_16, 0);
    lv_label_set_text(time_label, "00:00");
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 5, 0);

    // 充电状态
    charge_status_icon = lv_label_create(status_bar);
    lv_obj_set_style_text_color(charge_status_icon, lv_color_hex(0x10f703), 0);
    lv_obj_set_style_text_font(charge_status_icon, &lv_font_siyuanheiti_16, 0);
    lv_label_set_text(charge_status_icon, " ");
    lv_obj_align(charge_status_icon, LV_ALIGN_RIGHT_MID, 5, 0);

    // 电池电量
    battery_icon = lv_label_create(status_bar);
    lv_obj_set_style_text_color(battery_icon, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(battery_icon, &lv_font_siyuanheiti_16, 0);
    lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_FULL);
    lv_obj_align_to(battery_icon, charge_status_icon, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    battery_level_label = lv_label_create(status_bar);
    lv_obj_set_style_text_color(battery_level_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(battery_level_label, &lv_font_siyuanheiti_16, 0);
    lv_label_set_text(battery_level_label, "---%");
    lv_obj_set_align(battery_level_label, LV_TEXT_ALIGN_RIGHT);
    lv_obj_align_to(battery_level_label, battery_icon, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    // WIFI图标
    wifi_icon = lv_label_create(status_bar);
    lv_obj_set_style_text_color(wifi_icon, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(wifi_icon, &lv_font_siyuanheiti_16, 0);
    lv_label_set_text(wifi_icon, " ");
    lv_obj_align_to(wifi_icon, battery_level_label, LV_ALIGN_OUT_LEFT_MID, -20, 0);
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
        ESP_LOGW("STATUS_BAR_WIFI", "Set WiFi icon: %s", connected ? "Connected" : "Disconnected");
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

void status_bar_set_battery_level(float level)
{
    if (battery_icon && battery_level_label)
    {
        char buf[8];
        int level_int = (int)level;
        snprintf(buf, sizeof(buf), "%d%%", level_int);
        _lock_acquire(&lvgl_api_lock);
        lv_label_set_text(battery_level_label, buf);
        if (level_int > 90)
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_FULL);
        }
        else if (level_int > 60)
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_3);
        }
        else if (level_int > 40)
        {
            lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_2);
        }
        else if (level_int > 10)
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

void status_bar_set_charge_state(bool charging)
{
    if (charge_status_icon)
    {
        _lock_acquire(&lvgl_api_lock);
        if (charging)
        {
            lv_label_set_text(charge_status_icon, LV_SYMBOL_CHARGE);
        }
        else
        {
            lv_label_set_text(charge_status_icon, " ");
        }
        _lock_release(&lvgl_api_lock);
    }
}