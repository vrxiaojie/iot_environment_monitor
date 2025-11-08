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



void setup_scr_weather_screen(lv_ui *ui)
{
    //Write codes weather_screen
    ui->weather_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->weather_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->weather_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_cont_1
    ui->weather_screen_cont_1 = lv_obj_create(ui->weather_screen);
    lv_obj_set_pos(ui->weather_screen_cont_1, 0, 32);
    lv_obj_set_size(ui->weather_screen_cont_1, 480, 230);
    lv_obj_set_scrollbar_mode(ui->weather_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_cont_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_label_12
    ui->weather_screen_label_12 = lv_label_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_label_12, 31, 6);
    lv_obj_set_size(ui->weather_screen_label_12, 166, 32);
    lv_label_set_text(ui->weather_screen_label_12, "id:");
    lv_label_set_long_mode(ui->weather_screen_label_12, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_label_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_label_12, &lv_font_siyuanheiti_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_label_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_label_12, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_weather_label
    ui->weather_screen_weather_label = lv_label_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_weather_label, 227, 6);
    lv_obj_set_size(ui->weather_screen_weather_label, 119, 32);
    lv_label_set_text(ui->weather_screen_weather_label, "晴\n\n多云少间阴阵雨强雷伴有冰雹小中大极端降毛细暴特冻到雪夹天气薄雾霾扬沙浮尘浓度重严热冷");
    lv_label_set_long_mode(ui->weather_screen_weather_label, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_weather_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_weather_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_weather_label, &lv_font_siyuanheiti_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_weather_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_weather_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_weather_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_temp_cont
    ui->weather_screen_temp_cont = lv_obj_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_temp_cont, 5, 45);
    lv_obj_set_size(ui->weather_screen_temp_cont, 150, 80);
    lv_obj_set_scrollbar_mode(ui->weather_screen_temp_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen_temp_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_temp_cont, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_temp_cont, 27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_temp_cont, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_temp_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_temp_value
    ui->weather_screen_temp_value = lv_label_create(ui->weather_screen_temp_cont);
    lv_obj_set_pos(ui->weather_screen_temp_value, 15, 34);
    lv_obj_set_size(ui->weather_screen_temp_value, 120, 40);
    lv_label_set_text(ui->weather_screen_temp_value, "----");
    lv_label_set_long_mode(ui->weather_screen_temp_value, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_temp_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_temp_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_temp_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_temp_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_temp_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_label_1
    ui->weather_screen_label_1 = lv_label_create(ui->weather_screen_temp_cont);
    lv_obj_set_pos(ui->weather_screen_label_1, 26, 10);
    lv_obj_set_size(ui->weather_screen_label_1, 100, 22);
    lv_label_set_text(ui->weather_screen_label_1, "温度 ℃");
    lv_label_set_long_mode(ui->weather_screen_label_1, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_label_1, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_label_1, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_humid_cont
    ui->weather_screen_humid_cont = lv_obj_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_humid_cont, 165, 45);
    lv_obj_set_size(ui->weather_screen_humid_cont, 150, 80);
    lv_obj_set_scrollbar_mode(ui->weather_screen_humid_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen_humid_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_humid_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_humid_cont, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_humid_cont, 27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_humid_cont, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_humid_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_humid_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_humid_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_humid_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_humid_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_humid_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_humid_value
    ui->weather_screen_humid_value = lv_label_create(ui->weather_screen_humid_cont);
    lv_obj_set_pos(ui->weather_screen_humid_value, 15, 34);
    lv_obj_set_size(ui->weather_screen_humid_value, 120, 40);
    lv_label_set_text(ui->weather_screen_humid_value, "----");
    lv_label_set_long_mode(ui->weather_screen_humid_value, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_humid_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_humid_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_humid_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_humid_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_humid_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_label_3
    ui->weather_screen_label_3 = lv_label_create(ui->weather_screen_humid_cont);
    lv_obj_set_pos(ui->weather_screen_label_3, 26, 10);
    lv_obj_set_size(ui->weather_screen_label_3, 100, 22);
    lv_label_set_text(ui->weather_screen_label_3, "湿度 %");
    lv_label_set_long_mode(ui->weather_screen_label_3, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_label_3, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_label_3, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_wind_speed_cont
    ui->weather_screen_wind_speed_cont = lv_obj_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_wind_speed_cont, 325, 45);
    lv_obj_set_size(ui->weather_screen_wind_speed_cont, 150, 80);
    lv_obj_set_scrollbar_mode(ui->weather_screen_wind_speed_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen_wind_speed_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_wind_speed_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_wind_speed_cont, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_wind_speed_cont, 27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_wind_speed_cont, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_wind_speed_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_wind_speed_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_wind_speed_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_wind_speed_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_wind_speed_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_wind_speed_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_wind_speed_value
    ui->weather_screen_wind_speed_value = lv_label_create(ui->weather_screen_wind_speed_cont);
    lv_obj_set_pos(ui->weather_screen_wind_speed_value, 15, 34);
    lv_obj_set_size(ui->weather_screen_wind_speed_value, 120, 40);
    lv_label_set_text(ui->weather_screen_wind_speed_value, "----");
    lv_label_set_long_mode(ui->weather_screen_wind_speed_value, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_wind_speed_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_wind_speed_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_wind_speed_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_wind_speed_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_wind_speed_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_wind_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_label_5
    ui->weather_screen_label_5 = lv_label_create(ui->weather_screen_wind_speed_cont);
    lv_obj_set_pos(ui->weather_screen_label_5, 26, 10);
    lv_obj_set_size(ui->weather_screen_label_5, 100, 22);
    lv_label_set_text(ui->weather_screen_label_5, "风速 km/h");
    lv_label_set_long_mode(ui->weather_screen_label_5, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_label_5, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_label_5, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_feel_temp_cont
    ui->weather_screen_feel_temp_cont = lv_obj_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_feel_temp_cont, 5, 145);
    lv_obj_set_size(ui->weather_screen_feel_temp_cont, 150, 80);
    lv_obj_set_scrollbar_mode(ui->weather_screen_feel_temp_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen_feel_temp_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_feel_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_feel_temp_cont, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_feel_temp_cont, 27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_feel_temp_cont, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_feel_temp_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_feel_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_feel_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_feel_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_feel_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_feel_temp_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_feel_temp_value
    ui->weather_screen_feel_temp_value = lv_label_create(ui->weather_screen_feel_temp_cont);
    lv_obj_set_pos(ui->weather_screen_feel_temp_value, 15, 34);
    lv_obj_set_size(ui->weather_screen_feel_temp_value, 120, 40);
    lv_label_set_text(ui->weather_screen_feel_temp_value, "----");
    lv_label_set_long_mode(ui->weather_screen_feel_temp_value, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_feel_temp_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_feel_temp_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_feel_temp_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_feel_temp_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_feel_temp_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_feel_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_label_11
    ui->weather_screen_label_11 = lv_label_create(ui->weather_screen_feel_temp_cont);
    lv_obj_set_pos(ui->weather_screen_label_11, 26, 10);
    lv_obj_set_size(ui->weather_screen_label_11, 100, 22);
    lv_label_set_text(ui->weather_screen_label_11, "体感 ℃");
    lv_label_set_long_mode(ui->weather_screen_label_11, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_label_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_label_11, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_label_11, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_pressure_cont
    ui->weather_screen_pressure_cont = lv_obj_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_pressure_cont, 165, 145);
    lv_obj_set_size(ui->weather_screen_pressure_cont, 150, 80);
    lv_obj_set_scrollbar_mode(ui->weather_screen_pressure_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen_pressure_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_pressure_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_pressure_cont, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_pressure_cont, 27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_pressure_cont, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_pressure_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_pressure_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_pressure_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_pressure_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_pressure_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_pressure_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_pressure_value
    ui->weather_screen_pressure_value = lv_label_create(ui->weather_screen_pressure_cont);
    lv_obj_set_pos(ui->weather_screen_pressure_value, 15, 34);
    lv_obj_set_size(ui->weather_screen_pressure_value, 120, 40);
    lv_label_set_text(ui->weather_screen_pressure_value, "----");
    lv_label_set_long_mode(ui->weather_screen_pressure_value, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_pressure_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_pressure_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_pressure_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_pressure_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_pressure_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_pressure_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_label_9
    ui->weather_screen_label_9 = lv_label_create(ui->weather_screen_pressure_cont);
    lv_obj_set_pos(ui->weather_screen_label_9, 26, 10);
    lv_obj_set_size(ui->weather_screen_label_9, 100, 22);
    lv_label_set_text(ui->weather_screen_label_9, "气压 hPa");
    lv_label_set_long_mode(ui->weather_screen_label_9, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_label_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_label_9, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_label_9, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_visi_cont
    ui->weather_screen_visi_cont = lv_obj_create(ui->weather_screen_cont_1);
    lv_obj_set_pos(ui->weather_screen_visi_cont, 325, 145);
    lv_obj_set_size(ui->weather_screen_visi_cont, 150, 80);
    lv_obj_set_scrollbar_mode(ui->weather_screen_visi_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for weather_screen_visi_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_visi_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_visi_cont, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_visi_cont, 27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_visi_cont, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_visi_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_visi_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_visi_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_visi_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_visi_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_visi_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_visi_value
    ui->weather_screen_visi_value = lv_label_create(ui->weather_screen_visi_cont);
    lv_obj_set_pos(ui->weather_screen_visi_value, 15, 34);
    lv_obj_set_size(ui->weather_screen_visi_value, 120, 40);
    lv_label_set_text(ui->weather_screen_visi_value, "----");
    lv_label_set_long_mode(ui->weather_screen_visi_value, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_visi_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_visi_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_visi_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_visi_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_visi_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_visi_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_label_7
    ui->weather_screen_label_7 = lv_label_create(ui->weather_screen_visi_cont);
    lv_obj_set_pos(ui->weather_screen_label_7, 26, 10);
    lv_obj_set_size(ui->weather_screen_label_7, 100, 22);
    lv_label_set_text(ui->weather_screen_label_7, "能见度 km");
    lv_label_set_long_mode(ui->weather_screen_label_7, LV_LABEL_LONG_WRAP);

    //Write style for weather_screen_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_label_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_label_7, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_label_7, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_refresh_btn
    ui->weather_screen_refresh_btn = lv_button_create(ui->weather_screen);
    lv_obj_set_pos(ui->weather_screen_refresh_btn, 425, 35);
    lv_obj_set_size(ui->weather_screen_refresh_btn, 32, 32);
    ui->weather_screen_refresh_btn_label = lv_label_create(ui->weather_screen_refresh_btn);
    lv_label_set_text(ui->weather_screen_refresh_btn_label, " " LV_SYMBOL_REFRESH " ");
    lv_label_set_long_mode(ui->weather_screen_refresh_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->weather_screen_refresh_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->weather_screen_refresh_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->weather_screen_refresh_btn_label, LV_PCT(100));

    //Write style for weather_screen_refresh_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_screen_refresh_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_refresh_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_refresh_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->weather_screen_refresh_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_refresh_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_refresh_btn, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->weather_screen_refresh_btn, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->weather_screen_refresh_btn, 182, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->weather_screen_refresh_btn, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->weather_screen_refresh_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->weather_screen_refresh_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_refresh_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_refresh_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_refresh_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_refresh_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes weather_screen_setting_btn
    ui->weather_screen_setting_btn = lv_button_create(ui->weather_screen);
    lv_obj_set_pos(ui->weather_screen_setting_btn, 374, 35);
    lv_obj_set_size(ui->weather_screen_setting_btn, 32, 32);
    ui->weather_screen_setting_btn_label = lv_label_create(ui->weather_screen_setting_btn);
    lv_label_set_text(ui->weather_screen_setting_btn_label, " " LV_SYMBOL_SETTINGS " ");
    lv_label_set_long_mode(ui->weather_screen_setting_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->weather_screen_setting_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->weather_screen_setting_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->weather_screen_setting_btn_label, LV_PCT(100));

    //Write style for weather_screen_setting_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->weather_screen_setting_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->weather_screen_setting_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->weather_screen_setting_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->weather_screen_setting_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->weather_screen_setting_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->weather_screen_setting_btn, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->weather_screen_setting_btn, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->weather_screen_setting_btn, 182, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->weather_screen_setting_btn, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->weather_screen_setting_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->weather_screen_setting_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->weather_screen_setting_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->weather_screen_setting_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->weather_screen_setting_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->weather_screen_setting_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of weather_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->weather_screen);

    //Init events for screen.
    events_init_weather_screen(ui);
}
