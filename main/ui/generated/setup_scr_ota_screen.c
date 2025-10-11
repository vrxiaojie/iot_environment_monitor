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



void setup_scr_ota_screen(lv_ui *ui)
{
    //Write codes ota_screen
    ui->ota_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->ota_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->ota_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for ota_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_wifi_container
    ui->ota_screen_wifi_container = lv_obj_create(ui->ota_screen);
    lv_obj_set_pos(ui->ota_screen_wifi_container, 30, 40);
    lv_obj_set_size(ui->ota_screen_wifi_container, 420, 180);
    lv_obj_set_scrollbar_mode(ui->ota_screen_wifi_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for ota_screen_wifi_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_wifi_container, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_wifi_container, 101, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_wifi_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_wifi_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_1
    ui->ota_screen_label_1 = lv_label_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_label_1, 17, 7);
    lv_obj_set_size(ui->ota_screen_label_1, 384, 26);
    lv_label_set_text(ui->ota_screen_label_1, "OTA");
    lv_label_set_long_mode(ui->ota_screen_label_1, LV_LABEL_LONG_SCROLL_CIRCULAR);

    //Write style for ota_screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->ota_screen_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_1, &lv_font_JetBrainsMono_Medium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_url_input
    ui->ota_screen_url_input = lv_textarea_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_url_input, 87, 44);
    lv_obj_set_size(ui->ota_screen_url_input, 302, 36);
    lv_textarea_set_text(ui->ota_screen_url_input, "");
    lv_textarea_set_placeholder_text(ui->ota_screen_url_input, "URL");
    lv_textarea_set_password_bullet(ui->ota_screen_url_input, "*");
    lv_textarea_set_password_mode(ui->ota_screen_url_input, false);
    lv_textarea_set_one_line(ui->ota_screen_url_input, false);
    lv_textarea_set_accepted_chars(ui->ota_screen_url_input, "");
    lv_textarea_set_max_length(ui->ota_screen_url_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->ota_screen_url_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for ota_screen_url_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_url_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_url_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_url_input, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_url_input, &lv_font_JetBrainsMono_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_url_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_url_input, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_url_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_url_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_url_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_url_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_url_input, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_url_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_url_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_url_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ota_screen_url_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_url_input, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_url_input, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_url_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_url_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_2
    ui->ota_screen_label_2 = lv_label_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_label_2, 16, 52);
    lv_obj_set_size(ui->ota_screen_label_2, 59, 20);
    lv_label_set_text(ui->ota_screen_label_2, "更新源");
    lv_label_set_long_mode(ui->ota_screen_label_2, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_2, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_3
    ui->ota_screen_label_3 = lv_label_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_label_3, 16, 94);
    lv_obj_set_size(ui->ota_screen_label_3, 74, 20);
    lv_label_set_text(ui->ota_screen_label_3, "当前版本");
    lv_label_set_long_mode(ui->ota_screen_label_3, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_3, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_4
    ui->ota_screen_label_4 = lv_label_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_label_4, 106, 94);
    lv_obj_set_size(ui->ota_screen_label_4, 74, 20);
    lv_label_set_text(ui->ota_screen_label_4, "v1.x.x");
    lv_label_set_long_mode(ui->ota_screen_label_4, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_label_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_4, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_6
    ui->ota_screen_label_6 = lv_label_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_label_6, 206, 94);
    lv_obj_set_size(ui->ota_screen_label_6, 74, 20);
    lv_label_set_text(ui->ota_screen_label_6, "最新版本");
    lv_label_set_long_mode(ui->ota_screen_label_6, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_label_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_6, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_6, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_5
    ui->ota_screen_label_5 = lv_label_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_label_5, 296, 94);
    lv_obj_set_size(ui->ota_screen_label_5, 74, 20);
    lv_label_set_text(ui->ota_screen_label_5, "v1.x.x");
    lv_label_set_long_mode(ui->ota_screen_label_5, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_5, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_updateing_label
    ui->ota_screen_updateing_label = lv_label_create(ui->ota_screen_wifi_container);
    lv_obj_set_pos(ui->ota_screen_updateing_label, 79, 132);
    lv_obj_set_size(ui->ota_screen_updateing_label, 282, 20);
    lv_label_set_text(ui->ota_screen_updateing_label, "更新中，请勿关闭电源\n更新失败，请检查网络连接当前已是最新版本\n");
    lv_label_set_long_mode(ui->ota_screen_updateing_label, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_updateing_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_updateing_label, lv_color_hex(0xff7300), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_updateing_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_updateing_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_updateing_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_updateing_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_cont_1
    ui->ota_screen_cont_1 = lv_obj_create(ui->ota_screen);
    lv_obj_set_pos(ui->ota_screen_cont_1, 40, 223);
    lv_obj_set_size(ui->ota_screen_cont_1, 400, 45);
    lv_obj_set_scrollbar_mode(ui->ota_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for ota_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_update_btn
    ui->ota_screen_update_btn = lv_button_create(ui->ota_screen_cont_1);
    lv_obj_set_pos(ui->ota_screen_update_btn, 230, 10);
    lv_obj_set_size(ui->ota_screen_update_btn, 120, 30);
    ui->ota_screen_update_btn_label = lv_label_create(ui->ota_screen_update_btn);
    lv_label_set_text(ui->ota_screen_update_btn_label, "检查更新\n立即更新");
    lv_label_set_long_mode(ui->ota_screen_update_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->ota_screen_update_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->ota_screen_update_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->ota_screen_update_btn_label, LV_PCT(100));

    //Write style for ota_screen_update_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_update_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_update_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_update_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ota_screen_update_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_update_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_update_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_update_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_update_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_update_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_update_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_return_btn
    ui->ota_screen_return_btn = lv_button_create(ui->ota_screen_cont_1);
    lv_obj_set_pos(ui->ota_screen_return_btn, 50, 10);
    lv_obj_set_size(ui->ota_screen_return_btn, 120, 30);
    ui->ota_screen_return_btn_label = lv_label_create(ui->ota_screen_return_btn);
    lv_label_set_text(ui->ota_screen_return_btn_label, "返回");
    lv_label_set_long_mode(ui->ota_screen_return_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->ota_screen_return_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->ota_screen_return_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->ota_screen_return_btn_label, LV_PCT(100));

    //Write style for ota_screen_return_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_return_btn, 109, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_return_btn, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_return_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ota_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_return_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_return_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_return_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_return_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_return_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_update_percent_bar
    ui->ota_screen_update_percent_bar = lv_bar_create(ui->ota_screen);
    lv_obj_set_pos(ui->ota_screen_update_percent_bar, 103, 198);
    lv_obj_set_size(ui->ota_screen_update_percent_bar, 293, 10);
    lv_obj_set_style_anim_duration(ui->ota_screen_update_percent_bar, 1000, 0);
    lv_bar_set_mode(ui->ota_screen_update_percent_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->ota_screen_update_percent_bar, 0, 100);
    lv_bar_set_value(ui->ota_screen_update_percent_bar, 20, LV_ANIM_ON);

    //Write style for ota_screen_update_percent_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_update_percent_bar, 53, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_update_percent_bar, lv_color_hex(0x1a7f14), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_update_percent_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_update_percent_bar, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_update_percent_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ota_screen_update_percent_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_update_percent_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_update_percent_bar, lv_color_hex(0x26c961), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_update_percent_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_update_percent_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //The custom code of ota_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->ota_screen);

    //Init events for screen.
    events_init_ota_screen(ui);
}
