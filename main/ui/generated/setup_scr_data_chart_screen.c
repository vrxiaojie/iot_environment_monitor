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



void setup_scr_data_chart_screen(lv_ui *ui)
{
    //Write codes data_chart_screen
    ui->data_chart_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->data_chart_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->data_chart_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for data_chart_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->data_chart_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->data_chart_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->data_chart_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_btn_1min
    ui->data_chart_screen_btn_1min = lv_button_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_btn_1min, 30, 232);
    lv_obj_set_size(ui->data_chart_screen_btn_1min, 120, 30);
    ui->data_chart_screen_btn_1min_label = lv_label_create(ui->data_chart_screen_btn_1min);
    lv_label_set_text(ui->data_chart_screen_btn_1min_label, "1分钟");
    lv_label_set_long_mode(ui->data_chart_screen_btn_1min_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->data_chart_screen_btn_1min_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->data_chart_screen_btn_1min, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->data_chart_screen_btn_1min_label, LV_PCT(100));

    //Write style for data_chart_screen_btn_1min, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->data_chart_screen_btn_1min, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->data_chart_screen_btn_1min, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->data_chart_screen_btn_1min, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->data_chart_screen_btn_1min, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_btn_1min, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_btn_1min, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_btn_1min, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_btn_1min, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_btn_1min, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_btn_1min, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_btn_1h
    ui->data_chart_screen_btn_1h = lv_button_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_btn_1h, 180, 232);
    lv_obj_set_size(ui->data_chart_screen_btn_1h, 120, 30);
    ui->data_chart_screen_btn_1h_label = lv_label_create(ui->data_chart_screen_btn_1h);
    lv_label_set_text(ui->data_chart_screen_btn_1h_label, "1小时");
    lv_label_set_long_mode(ui->data_chart_screen_btn_1h_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->data_chart_screen_btn_1h_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->data_chart_screen_btn_1h, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->data_chart_screen_btn_1h_label, LV_PCT(100));

    //Write style for data_chart_screen_btn_1h, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->data_chart_screen_btn_1h, 109, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->data_chart_screen_btn_1h, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->data_chart_screen_btn_1h, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->data_chart_screen_btn_1h, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_btn_1h, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_btn_1h, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_btn_1h, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_btn_1h, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_btn_1h, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_btn_1h, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_btn_24h
    ui->data_chart_screen_btn_24h = lv_button_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_btn_24h, 330, 232);
    lv_obj_set_size(ui->data_chart_screen_btn_24h, 120, 30);
    ui->data_chart_screen_btn_24h_label = lv_label_create(ui->data_chart_screen_btn_24h);
    lv_label_set_text(ui->data_chart_screen_btn_24h_label, "24小时");
    lv_label_set_long_mode(ui->data_chart_screen_btn_24h_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->data_chart_screen_btn_24h_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->data_chart_screen_btn_24h, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->data_chart_screen_btn_24h_label, LV_PCT(100));

    //Write style for data_chart_screen_btn_24h, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->data_chart_screen_btn_24h, 109, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->data_chart_screen_btn_24h, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->data_chart_screen_btn_24h, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->data_chart_screen_btn_24h, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_btn_24h, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_btn_24h, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_btn_24h, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_btn_24h, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_btn_24h, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_btn_24h, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_btn_back
    ui->data_chart_screen_btn_back = lv_button_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_btn_back, 10, 8);
    lv_obj_set_size(ui->data_chart_screen_btn_back, 85, 24);
    ui->data_chart_screen_btn_back_label = lv_label_create(ui->data_chart_screen_btn_back);
    lv_label_set_text(ui->data_chart_screen_btn_back_label, "←");
    lv_label_set_long_mode(ui->data_chart_screen_btn_back_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->data_chart_screen_btn_back_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->data_chart_screen_btn_back, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->data_chart_screen_btn_back_label, LV_PCT(100));

    //Write style for data_chart_screen_btn_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->data_chart_screen_btn_back, 102, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->data_chart_screen_btn_back, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->data_chart_screen_btn_back, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->data_chart_screen_btn_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_btn_back, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_btn_back, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->data_chart_screen_btn_back, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->data_chart_screen_btn_back, 182, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->data_chart_screen_btn_back, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->data_chart_screen_btn_back, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->data_chart_screen_btn_back, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_btn_back, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_btn_back, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_btn_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_btn_back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_title_label
    ui->data_chart_screen_title_label = lv_label_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_title_label, 95, 14);
    lv_obj_set_size(ui->data_chart_screen_title_label, 151, 18);
    lv_label_set_text(ui->data_chart_screen_title_label, "xxxx 历史数据");
    lv_label_set_long_mode(ui->data_chart_screen_title_label, LV_LABEL_LONG_WRAP);

    //Write style for data_chart_screen_title_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_title_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_title_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_title_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_title_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_title_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_date_label
    ui->data_chart_screen_date_label = lv_label_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_date_label, 260, 6);
    lv_obj_set_size(ui->data_chart_screen_date_label, 46, 20);
    lv_label_set_text(ui->data_chart_screen_date_label, "--/--");
    lv_label_set_long_mode(ui->data_chart_screen_date_label, LV_LABEL_LONG_WRAP);

    //Write style for data_chart_screen_date_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_date_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_date_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_date_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_date_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_time_label
    ui->data_chart_screen_time_label = lv_label_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_time_label, 260, 24);
    lv_obj_set_size(ui->data_chart_screen_time_label, 46, 20);
    lv_label_set_text(ui->data_chart_screen_time_label, "--:--");
    lv_label_set_long_mode(ui->data_chart_screen_time_label, LV_LABEL_LONG_WRAP);

    //Write style for data_chart_screen_time_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_time_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_time_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_time_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_time_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes data_chart_screen_data_label
    ui->data_chart_screen_data_label = lv_label_create(ui->data_chart_screen);
    lv_obj_set_pos(ui->data_chart_screen_data_label, 330, 8);
    lv_obj_set_size(ui->data_chart_screen_data_label, 133, 32);
    lv_label_set_text(ui->data_chart_screen_data_label, "---- C");
    lv_label_set_long_mode(ui->data_chart_screen_data_label, LV_LABEL_LONG_WRAP);

    //Write style for data_chart_screen_data_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->data_chart_screen_data_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->data_chart_screen_data_label, &lv_font_JetBrainsMono_Medium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->data_chart_screen_data_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->data_chart_screen_data_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->data_chart_screen_data_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of data_chart_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->data_chart_screen);

    //Init events for screen.
    events_init_data_chart_screen(ui);
}
