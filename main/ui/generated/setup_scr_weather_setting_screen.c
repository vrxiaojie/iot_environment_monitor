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



void setup_scr_weather_setting_screen(lv_ui *ui)
{
    //Write codes weather_setting_screen
    ui->weather_setting_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->weather_setting_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->weather_setting_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_setting_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_setting_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_cont_1
    ui->weather_setting_screen_cont_1 = lv_obj_create(ui->weather_setting_screen);
    lv_obj_set_pos(ui->weather_setting_screen_cont_1, 30, 32);
    lv_obj_set_size(ui->weather_setting_screen_cont_1, 420, 180);
    lv_obj_set_scrollbar_mode(ui->weather_setting_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_setting_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_cont_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_cont_1, 101, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_cont_1, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_label_14
    ui->weather_setting_screen_label_14 = lv_label_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_label_14, 10, 8);
    lv_obj_set_size(ui->weather_setting_screen_label_14, 107, 20);
    lv_label_set_text(ui->weather_setting_screen_label_14, "和风天气设置");
    lv_label_set_long_mode(ui->weather_setting_screen_label_14, LV_LABEL_LONG_WRAP);

    //Write style for weather_setting_screen_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_setting_screen_label_14, lv_color_hex(0x26c961), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_label_14, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_label_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_label_14, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_host_input
    ui->weather_setting_screen_host_input = lv_textarea_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_host_input, 70, 30);
    lv_obj_set_size(ui->weather_setting_screen_host_input, 161, 36);
    lv_textarea_set_text(ui->weather_setting_screen_host_input, "");
    lv_textarea_set_placeholder_text(ui->weather_setting_screen_host_input, "xxx.qweather.com");
    lv_textarea_set_password_bullet(ui->weather_setting_screen_host_input, "*");
    lv_textarea_set_password_mode(ui->weather_setting_screen_host_input, false);
    lv_textarea_set_one_line(ui->weather_setting_screen_host_input, true);
    lv_textarea_set_accepted_chars(ui->weather_setting_screen_host_input, "");
    lv_textarea_set_max_length(ui->weather_setting_screen_host_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->weather_setting_screen_host_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for weather_setting_screen_host_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->weather_setting_screen_host_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_host_input, &lv_font_siyuanheiti_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_host_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_host_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_host_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_host_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_host_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_host_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->weather_setting_screen_host_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_host_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_host_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_host_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_host_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_host_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for weather_setting_screen_host_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_host_input, 104, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_host_input, lv_color_hex(0xffffff), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_host_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_host_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_label_15
    ui->weather_setting_screen_label_15 = lv_label_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_label_15, 20, 38);
    lv_obj_set_size(ui->weather_setting_screen_label_15, 43, 20);
    lv_label_set_text(ui->weather_setting_screen_label_15, "Host");
    lv_label_set_long_mode(ui->weather_setting_screen_label_15, LV_LABEL_LONG_WRAP);

    //Write style for weather_setting_screen_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_setting_screen_label_15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_label_15, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_label_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_label_15, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_label_16
    ui->weather_setting_screen_label_16 = lv_label_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_label_16, 10, 93);
    lv_obj_set_size(ui->weather_setting_screen_label_16, 60, 20);
    lv_label_set_text(ui->weather_setting_screen_label_16, "API key");
    lv_label_set_long_mode(ui->weather_setting_screen_label_16, LV_LABEL_LONG_WRAP);

    //Write style for weather_setting_screen_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_setting_screen_label_16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_label_16, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_label_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_label_16, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_apikey_input
    ui->weather_setting_screen_apikey_input = lv_textarea_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_apikey_input, 70, 85);
    lv_obj_set_size(ui->weather_setting_screen_apikey_input, 341, 36);
    lv_textarea_set_text(ui->weather_setting_screen_apikey_input, "");
    lv_textarea_set_placeholder_text(ui->weather_setting_screen_apikey_input, "Hefeng API Key");
    lv_textarea_set_password_bullet(ui->weather_setting_screen_apikey_input, "*");
    lv_textarea_set_password_mode(ui->weather_setting_screen_apikey_input, false);
    lv_textarea_set_one_line(ui->weather_setting_screen_apikey_input, true);
    lv_textarea_set_accepted_chars(ui->weather_setting_screen_apikey_input, "");
    lv_textarea_set_max_length(ui->weather_setting_screen_apikey_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->weather_setting_screen_apikey_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for weather_setting_screen_apikey_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->weather_setting_screen_apikey_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_apikey_input, &lv_font_siyuanheiti_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_apikey_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_apikey_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_apikey_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_apikey_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_apikey_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_apikey_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->weather_setting_screen_apikey_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_apikey_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_apikey_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_apikey_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_apikey_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_apikey_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for weather_setting_screen_apikey_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_apikey_input, 104, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_apikey_input, lv_color_hex(0xffffff), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_apikey_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_apikey_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_label_17
    ui->weather_setting_screen_label_17 = lv_label_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_label_17, 236, 31);
    lv_obj_set_size(ui->weather_setting_screen_label_17, 76, 44);
    lv_label_set_text(ui->weather_setting_screen_label_17, "      城市\n(支持拼音)");
    lv_label_set_long_mode(ui->weather_setting_screen_label_17, LV_LABEL_LONG_WRAP);

    //Write style for weather_setting_screen_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_setting_screen_label_17, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_label_17, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_label_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_label_17, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_city_input
    ui->weather_setting_screen_city_input = lv_textarea_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_city_input, 316, 30);
    lv_obj_set_size(ui->weather_setting_screen_city_input, 94, 36);
    lv_textarea_set_text(ui->weather_setting_screen_city_input, "");
    lv_textarea_set_placeholder_text(ui->weather_setting_screen_city_input, "city");
    lv_textarea_set_password_bullet(ui->weather_setting_screen_city_input, "*");
    lv_textarea_set_password_mode(ui->weather_setting_screen_city_input, false);
    lv_textarea_set_one_line(ui->weather_setting_screen_city_input, true);
    lv_textarea_set_accepted_chars(ui->weather_setting_screen_city_input, "");
    lv_textarea_set_max_length(ui->weather_setting_screen_city_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->weather_setting_screen_city_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for weather_setting_screen_city_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->weather_setting_screen_city_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_city_input, &lv_font_siyuanheiti_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_city_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_city_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_city_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_city_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_city_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_city_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->weather_setting_screen_city_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_city_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_city_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_city_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_city_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_city_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for weather_setting_screen_city_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_city_input, 104, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_city_input, lv_color_hex(0xffffff), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_city_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_city_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_label_18
    ui->weather_setting_screen_label_18 = lv_label_create(ui->weather_setting_screen_cont_1);
    lv_obj_set_pos(ui->weather_setting_screen_label_18, 94, 135);
    lv_obj_set_size(ui->weather_setting_screen_label_18, 289, 44);
    lv_label_set_text(ui->weather_setting_screen_label_18, "和风天气Host和APIkey的获取方法\n请参考wiki.vrxiaojie.top中的教程");
    lv_label_set_long_mode(ui->weather_setting_screen_label_18, LV_LABEL_LONG_WRAP);

    //Write style for weather_setting_screen_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_setting_screen_label_18, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_label_18, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_label_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_label_18, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_save_btn
    ui->weather_setting_screen_save_btn = lv_button_create(ui->weather_setting_screen);
    lv_obj_set_pos(ui->weather_setting_screen_save_btn, 280, 230);
    lv_obj_set_size(ui->weather_setting_screen_save_btn, 120, 30);
    ui->weather_setting_screen_save_btn_label = lv_label_create(ui->weather_setting_screen_save_btn);
    lv_label_set_text(ui->weather_setting_screen_save_btn_label, "保存");
    lv_label_set_long_mode(ui->weather_setting_screen_save_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->weather_setting_screen_save_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->weather_setting_screen_save_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->weather_setting_screen_save_btn_label, LV_PCT(100));

    //Write style for weather_setting_screen_save_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_save_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_save_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_save_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->weather_setting_screen_save_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_save_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_save_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_setting_screen_save_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_save_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_save_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_save_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_setting_screen_return_btn
    ui->weather_setting_screen_return_btn = lv_button_create(ui->weather_setting_screen);
    lv_obj_set_pos(ui->weather_setting_screen_return_btn, 80, 230);
    lv_obj_set_size(ui->weather_setting_screen_return_btn, 120, 30);
    ui->weather_setting_screen_return_btn_label = lv_label_create(ui->weather_setting_screen_return_btn);
    lv_label_set_text(ui->weather_setting_screen_return_btn_label, "返回");
    lv_label_set_long_mode(ui->weather_setting_screen_return_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->weather_setting_screen_return_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->weather_setting_screen_return_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->weather_setting_screen_return_btn_label, LV_PCT(100));

    //Write style for weather_setting_screen_return_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_setting_screen_return_btn, 109, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_setting_screen_return_btn, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_setting_screen_return_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->weather_setting_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_setting_screen_return_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_setting_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_setting_screen_return_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_setting_screen_return_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_setting_screen_return_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_setting_screen_return_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of weather_setting_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->weather_setting_screen);

    //Init events for screen.
    events_init_weather_setting_screen(ui);
}
