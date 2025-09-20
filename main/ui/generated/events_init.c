/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

uint8_t wifi_status = 0;
uint8_t bluetooth_status = 0;
uint8_t powerSaveMode_status = 0;
#ifndef LV_USE_GUIDER_SIMULATOR
#include "esp_lcd_panel_rgb.h"

extern esp_lcd_panel_handle_t panel_handle;
#endif
#ifndef LV_USE_GUIDER_SIMULATOR
#include "backlight.h"

uint8_t backlight;
#endif
#ifndef LV_USE_GUIDER_SIMULATOR
#include "wifi.h"
#endif
#ifndef LV_USE_GUIDER_SIMULATOR
// 单实例网络信息消息框指针
static lv_obj_t *network_info_msgbox = NULL;
static lv_obj_t *network_info_msgbox_label = NULL;

static void network_info_msgbox_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_DELETE) {
        network_info_msgbox = NULL;
        network_info_msgbox_label = NULL;
    }
}
#endif

static void main_screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.setting_screen, guider_ui.setting_screen_del, &guider_ui.main_screen_del, setup_scr_setting_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void main_screen_voc_container_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        break;
    }
    default:
        break;
    }
}

void events_init_main_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->main_screen, main_screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_screen_voc_container, main_screen_voc_container_event_handler, LV_EVENT_ALL, ui);
}

static void setting_screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        // 在这里对slider的值做调整
#ifndef LV_USE_GUIDER_SIMULATOR
        lcd_backlight_get_duty(&backlight);
        lv_slider_set_value(guider_ui.setting_screen_backlight_slider, backlight, LV_ANIM_OFF);
#endif

        // 根据wifi状态调整wifi图标样式
        if (wifi_status == 0) {
            lv_obj_set_style_image_recolor(guider_ui.setting_screen_wifi_icon, lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_set_style_bg_color(guider_ui.setting_screen_wifi_icon_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(guider_ui.setting_screen_wifi_icon_container, 128, LV_PART_MAIN);
        } else if (wifi_status == 1) {
            lv_obj_set_style_image_recolor(guider_ui.setting_screen_wifi_icon, lv_color_hex(0x1296db), LV_PART_MAIN);
            lv_obj_set_style_bg_color(guider_ui.setting_screen_wifi_icon_container, lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(guider_ui.setting_screen_wifi_icon_container, 255, LV_PART_MAIN);
        }
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.main_screen, guider_ui.main_screen_del, &guider_ui.setting_screen_del, setup_scr_main_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void setting_screen_wifi_icon_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SHORT_CLICKED:
    {
        if (wifi_status == 0) {
            lv_obj_set_style_image_recolor(guider_ui.setting_screen_wifi_icon, lv_color_hex(0x1296db), LV_PART_MAIN);
            lv_obj_set_style_bg_color(guider_ui.setting_screen_wifi_icon_container, lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(guider_ui.setting_screen_wifi_icon_container, 255, LV_PART_MAIN);
            wifi_status = 1;
#ifndef LV_USE_GUIDER_SIMULATOR
            wifi_start();
            wifi_connect_to_saved_ap();
#endif
        } else if (wifi_status == 1) {
            lv_obj_set_style_image_recolor(guider_ui.setting_screen_wifi_icon, lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_set_style_bg_color(guider_ui.setting_screen_wifi_icon_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(guider_ui.setting_screen_wifi_icon_container, 128, LV_PART_MAIN);
            wifi_status = 0;

#ifndef LV_USE_GUIDER_SIMULATOR
            wifi_stop();
            esp_lcd_rgb_panel_set_pclk(panel_handle, 10 * 1000 * 1000);
#endif
        }
        break;
    }
    case LV_EVENT_LONG_PRESSED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.wifi_setting_screen, guider_ui.wifi_setting_screen_del, &guider_ui.setting_screen_del, setup_scr_wifi_setting_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false, false);
        break;
    }
    default:
        break;
    }
}

static void setting_screen_backlight_slider_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
#ifndef LV_USE_GUIDER_SIMULATOR
        lv_obj_t * slider = lv_event_get_target(e);
        lcd_backlight_set_duty(lv_slider_get_value(slider));
#endif
        break;
    }
    default:
        break;
    }
}

void events_init_setting_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->setting_screen, setting_screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_screen_wifi_icon, setting_screen_wifi_icon_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->setting_screen_backlight_slider, setting_screen_backlight_slider_event_handler, LV_EVENT_ALL, ui);
}

static void wifi_setting_screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        if (wifi_sta_status == WIFI_CONNECTED)
        {
            lv_obj_set_style_text_color(guider_ui.wifi_setting_screen_connect_status_label, lv_color_hex(0x26c961), LV_PART_MAIN);
        }
        else if (wifi_sta_status == WIFI_DISCONNECTED )
        {
            lv_obj_set_style_text_color(guider_ui.wifi_setting_screen_connect_status_label, lv_color_hex(0xE8202D), LV_PART_MAIN);
        }
        if (wifi_status == 0) {
            lv_obj_clear_state(guider_ui.wifi_setting_screen_wifi_switch, LV_STATE_CHECKED);
        } else if (wifi_status == 1) {
            lv_obj_add_state(guider_ui.wifi_setting_screen_wifi_switch, LV_STATE_CHECKED);
        }
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.setting_screen, guider_ui.setting_screen_del, &guider_ui.wifi_setting_screen_del, setup_scr_setting_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false, false);
            break;
        }
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.setting_screen, guider_ui.setting_screen_del, &guider_ui.wifi_setting_screen_del, setup_scr_setting_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void wifi_setting_screen_wifi_switch_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        lv_obj_remove_flag(guider_ui.wifi_setting_screen_wifi_scan_spinner, LV_OBJ_FLAG_HIDDEN);

        switch (status) {
        case (true):
        {
            // 将wifi开启状态置为1
            wifi_status = 1;

#ifndef LV_USE_GUIDER_SIMULATOR
            wifi_start();
            wifi_scan();
#endif
            break;
        }
        case (false):
        {
            // 将wifi开启状态置为0
            wifi_status = 0;

#ifndef LV_USE_GUIDER_SIMULATOR
            wifi_stop();
            // 清空wifi列表
            if (guider_ui.wifi_setting_screen_wifi_scan_list)
            {
                lv_obj_clean(guider_ui.wifi_setting_screen_wifi_scan_list);
            }
            esp_lcd_rgb_panel_set_pclk(panel_handle, 10 * 1000 * 1000);
#endif
            lv_obj_add_flag(guider_ui.wifi_setting_screen_wifi_scan_spinner, LV_OBJ_FLAG_HIDDEN);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void wifi_setting_screen_network_info_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifndef LV_USE_GUIDER_SIMULATOR
        char t[64];
        wifi_ip_info_t wifi_ip_info;
        wifi_get_ip_info_str(&wifi_ip_info);
        snprintf(t, 64, "IP: %s\nMask: %s\nGW: %s\n",
                 wifi_ip_info.ip, wifi_ip_info.netmask, wifi_ip_info.gw);
        // 若已经存在，则更新内容并置顶，不再重复创建
        if (network_info_msgbox) {
            if (network_info_msgbox_label) {
                lv_label_set_text(network_info_msgbox_label, t);
            }
            lv_obj_move_foreground(network_info_msgbox);
            break;
        }
        network_info_msgbox = lv_msgbox_create(guider_ui.wifi_setting_screen);
        lv_obj_set_pos(network_info_msgbox, 95, 96);
        lv_obj_set_size(network_info_msgbox, 280, 150);
        lv_msgbox_add_title(network_info_msgbox, "Network Info");
        network_info_msgbox_label = lv_msgbox_add_text(network_info_msgbox, t);
        lv_obj_align_to(network_info_msgbox, guider_ui.wifi_setting_screen, LV_ALIGN_TOP_LEFT, 95, 96);
        lv_msgbox_add_close_button(network_info_msgbox);
        lv_obj_add_event_cb(network_info_msgbox, network_info_msgbox_event_cb, LV_EVENT_DELETE, NULL);
#endif
        break;
    }
    default:
        break;
    }
}

void events_init_wifi_setting_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->wifi_setting_screen, wifi_setting_screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->wifi_setting_screen_wifi_switch, wifi_setting_screen_wifi_switch_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->wifi_setting_screen_network_info_btn, wifi_setting_screen_network_info_btn_event_handler, LV_EVENT_ALL, ui);
}

static void wifi_connect_screen_conn_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifndef LV_USE_GUIDER_SIMULATOR
        user_wifi_cfg *cfg = (user_wifi_cfg *)malloc(sizeof(user_wifi_cfg));
        lv_memcpy(cfg->ssid, lv_label_get_text(guider_ui.wifi_connect_screen_ssid), 32);
        lv_memcpy(cfg->password, lv_textarea_get_text(guider_ui.wifi_connect_screen_password_input), 64);
        wifi_connect(cfg);
        ui_load_scr_animation(&guider_ui, &guider_ui.wifi_setting_screen, guider_ui.wifi_setting_screen_del, &guider_ui.wifi_connect_screen_del, setup_scr_wifi_setting_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0, false, true);
#endif

        break;
    }
    default:
        break;
    }
}

static void wifi_connect_screen_cancel_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.wifi_setting_screen, guider_ui.wifi_setting_screen_del, &guider_ui.wifi_connect_screen_del, setup_scr_wifi_setting_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_wifi_connect_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->wifi_connect_screen_conn_btn, wifi_connect_screen_conn_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->wifi_connect_screen_cancel_btn, wifi_connect_screen_cancel_btn_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
