/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_setting_screen(lv_ui *ui)
{
    //Write codes setting_screen
    ui->setting_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->setting_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->setting_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for setting_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_wifi_icon_container
    ui->setting_screen_wifi_icon_container = lv_obj_create(ui->setting_screen);
    lv_obj_set_pos(ui->setting_screen_wifi_icon_container, 62, 53);
    lv_obj_set_size(ui->setting_screen_wifi_icon_container, 64, 64);
    lv_obj_set_scrollbar_mode(ui->setting_screen_wifi_icon_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for setting_screen_wifi_icon_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_screen_wifi_icon_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_screen_wifi_icon_container, 48, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_screen_wifi_icon_container, 128, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_screen_wifi_icon_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_screen_wifi_icon_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_screen_wifi_icon_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_screen_wifi_icon_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_screen_wifi_icon_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_screen_wifi_icon_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_screen_wifi_icon_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_wifi_icon
    ui->setting_screen_wifi_icon = lv_image_create(ui->setting_screen_wifi_icon_container);
    lv_obj_set_pos(ui->setting_screen_wifi_icon, 8, 8);
    lv_obj_set_size(ui->setting_screen_wifi_icon, 48, 48);
    lv_obj_add_flag(ui->setting_screen_wifi_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->setting_screen_wifi_icon, &_WIFI_RGB565A8_48x48);
    lv_image_set_pivot(ui->setting_screen_wifi_icon, 50,50);
    lv_image_set_rotation(ui->setting_screen_wifi_icon, 0);

    //Write style for setting_screen_wifi_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->setting_screen_wifi_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->setting_screen_wifi_icon, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->setting_screen_wifi_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_backlight_container
    ui->setting_screen_backlight_container = lv_obj_create(ui->setting_screen);
    lv_obj_set_pos(ui->setting_screen_backlight_container, 404, 28);
    lv_obj_set_size(ui->setting_screen_backlight_container, 70, 220);
    lv_obj_set_scrollbar_mode(ui->setting_screen_backlight_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for setting_screen_backlight_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_screen_backlight_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_backlight_icon
    ui->setting_screen_backlight_icon = lv_image_create(ui->setting_screen_backlight_container);
    lv_obj_set_pos(ui->setting_screen_backlight_icon, 18, 180);
    lv_obj_set_size(ui->setting_screen_backlight_icon, 32, 32);
    lv_obj_add_flag(ui->setting_screen_backlight_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->setting_screen_backlight_icon, &_backlight_RGB565A8_32x32);
    lv_image_set_pivot(ui->setting_screen_backlight_icon, 50,50);
    lv_image_set_rotation(ui->setting_screen_backlight_icon, 0);

    //Write style for setting_screen_backlight_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->setting_screen_backlight_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->setting_screen_backlight_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_backlight_slider
    ui->setting_screen_backlight_slider = lv_slider_create(ui->setting_screen_backlight_container);
    lv_obj_set_pos(ui->setting_screen_backlight_slider, 26, 16);
    lv_obj_set_size(ui->setting_screen_backlight_slider, 16, 160);
    lv_slider_set_range(ui->setting_screen_backlight_slider, 10, 100);
    lv_slider_set_mode(ui->setting_screen_backlight_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->setting_screen_backlight_slider, 50, LV_ANIM_OFF);

    //Write style for setting_screen_backlight_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_screen_backlight_slider, 33, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_screen_backlight_slider, lv_color_hex(0x1a7f14), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_screen_backlight_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_screen_backlight_slider, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->setting_screen_backlight_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_screen_backlight_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for setting_screen_backlight_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_screen_backlight_slider, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_screen_backlight_slider, lv_color_hex(0x26c961), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_screen_backlight_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_screen_backlight_slider, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for setting_screen_backlight_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_screen_backlight_slider, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_screen_backlight_slider, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes setting_screen_bluetooth_container
    ui->setting_screen_bluetooth_container = lv_obj_create(ui->setting_screen);
    lv_obj_set_pos(ui->setting_screen_bluetooth_container, 184, 53);
    lv_obj_set_size(ui->setting_screen_bluetooth_container, 64, 64);
    lv_obj_set_scrollbar_mode(ui->setting_screen_bluetooth_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for setting_screen_bluetooth_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_screen_bluetooth_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_screen_bluetooth_container, 48, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_screen_bluetooth_container, 128, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_screen_bluetooth_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_screen_bluetooth_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_screen_bluetooth_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_screen_bluetooth_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_screen_bluetooth_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_screen_bluetooth_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_screen_bluetooth_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_bluetooth_icon
    ui->setting_screen_bluetooth_icon = lv_image_create(ui->setting_screen_bluetooth_container);
    lv_obj_set_pos(ui->setting_screen_bluetooth_icon, 8, 8);
    lv_obj_set_size(ui->setting_screen_bluetooth_icon, 48, 48);
    lv_obj_add_flag(ui->setting_screen_bluetooth_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->setting_screen_bluetooth_icon, &_bluetooth_RGB565A8_48x48);
    lv_image_set_pivot(ui->setting_screen_bluetooth_icon, 50,50);
    lv_image_set_rotation(ui->setting_screen_bluetooth_icon, 0);

    //Write style for setting_screen_bluetooth_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->setting_screen_bluetooth_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->setting_screen_bluetooth_icon, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->setting_screen_bluetooth_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_power_save_container
    ui->setting_screen_power_save_container = lv_obj_create(ui->setting_screen);
    lv_obj_set_pos(ui->setting_screen_power_save_container, 62, 159);
    lv_obj_set_size(ui->setting_screen_power_save_container, 64, 64);
    lv_obj_set_scrollbar_mode(ui->setting_screen_power_save_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for setting_screen_power_save_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_screen_power_save_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_screen_power_save_container, 48, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_screen_power_save_container, 128, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_screen_power_save_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_screen_power_save_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_screen_power_save_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_screen_power_save_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_screen_power_save_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_screen_power_save_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_screen_power_save_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_screen_power_save_icon
    ui->setting_screen_power_save_icon = lv_image_create(ui->setting_screen_power_save_container);
    lv_obj_set_pos(ui->setting_screen_power_save_icon, 9, 8);
    lv_obj_set_size(ui->setting_screen_power_save_icon, 48, 48);
    lv_obj_add_flag(ui->setting_screen_power_save_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->setting_screen_power_save_icon, &_power_save_mode_RGB565A8_48x48);
    lv_image_set_pivot(ui->setting_screen_power_save_icon, 50,50);
    lv_image_set_rotation(ui->setting_screen_power_save_icon, 0);

    //Write style for setting_screen_power_save_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->setting_screen_power_save_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->setting_screen_power_save_icon, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->setting_screen_power_save_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of setting_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->setting_screen);

    //Init events for screen.
    events_init_setting_screen(ui);
}
