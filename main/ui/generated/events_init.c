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
#ifndef LV_USE_GUIDER_SIMULATOR
#include "backlight.h"

uint8_t backlight;
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
    case LV_EVENT_CLICKED:
    {
        if (wifi_status == 0) {
            lv_obj_set_style_image_recolor(guider_ui.setting_screen_wifi_icon, lv_color_hex(0x1296db), LV_PART_MAIN);
            lv_obj_set_style_bg_color(guider_ui.setting_screen_wifi_icon_container, lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(guider_ui.setting_screen_wifi_icon_container, 255, LV_PART_MAIN);
            wifi_status = 1;
        } else if (wifi_status == 1) {
            lv_obj_set_style_image_recolor(guider_ui.setting_screen_wifi_icon, lv_color_hex(0xffffff), LV_PART_MAIN);
            lv_obj_set_style_bg_color(guider_ui.setting_screen_wifi_icon_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(guider_ui.setting_screen_wifi_icon_container, 128, LV_PART_MAIN);
            wifi_status = 0;
        }
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


void events_init(lv_ui *ui)
{

}
