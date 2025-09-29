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
#ifndef LV_USE_GUIDER_SIMULATOR
#define DARK_GREEN (0x1a7f14)
#define GREEN (0x10f703)
#define BRIGHT_GREEN (0x26c961)
#define YELLOW_GREEN (0xB0CE20)
#define DARK_YELLOW (0xFFC000)
#define YELLOW (0xFFF100)
#define BRIGHT_YELLOW (0xefff3d)
#define ORANGE (0xff7300)
#define BRIGHT_ORANGE (0xffac03)
#define RED (0xE8202D)
#define BRIGHT_RED (0xf20303)
#define DARK_PURPLE (0xA42C9E)
#define BRIGHT_PURPLE (0xf53dff)
#define GRAY (0x5b5b5b)
#define DARK_BLUE (0x0f4187)
#define BLUE (0x0d6bfd)
#define BRIGHT_BLUE (0x11c0f5)

lv_timer_t *update_data_timer = NULL;

static void update_voc_display() {
    static int32_t voc_idx;
    char t[12] = {0};

    extern int32_t voc_index;
    voc_idx = voc_index;

    if(voc_idx >= 0)
        sprintf(t,"%ld",voc_idx);
    else
        sprintf(t,"----");
    lv_label_set_text(guider_ui.main_screen_voc_value, t);
    // VOC 0~100 设置背景颜色
    if(voc_idx >=0 && voc_idx <= 100 ) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_container, lv_color_hex(DARK_GREEN), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_bar, lv_color_hex(BRIGHT_GREEN), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(voc_idx >=101 && voc_idx <= 300) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_container, lv_color_hex(DARK_YELLOW), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_bar, lv_color_hex(BRIGHT_YELLOW), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(voc_idx >= 301) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_container, lv_color_hex(DARK_PURPLE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_bar, lv_color_hex(BRIGHT_PURPLE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else {
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_container, lv_color_hex(GRAY), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_voc_bar, lv_color_hex(GRAY), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }

}

#include "stcc4.h"

static void update_co2_temp_humid_display() {
    static int16_t co2 = 0;
    static int16_t temp = -400;
    static uint16_t humid = 100;
    char t[12];

    extern STCC4_t stcc4;
    co2 = stcc4.co2Concentration;
    temp = stcc4.temperature * 10;
    humid = stcc4.relativeHumidity * 10;

    // 处理co2数值
    if(co2 >= 0 && co2 <= 30000)
        sprintf(t,"%d",co2);
    else
        sprintf(t,"----");
    lv_label_set_text(guider_ui.main_screen_co2_value, t);

    // co2 设置背景颜色
    if(co2 >= 0 && co2 <= 450 ) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_container, lv_color_hex(DARK_GREEN), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_bar, lv_color_hex(GREEN), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(co2 >= 451 && co2 <= 700) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_container, lv_color_hex(GREEN), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_bar, lv_color_hex(BRIGHT_GREEN), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(co2 >= 701 && co2 <= 1000) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_container, lv_color_hex(YELLOW_GREEN), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_bar, lv_color_hex(YELLOW_GREEN), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(co2 >= 1001 && co2 <= 2500) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_container, lv_color_hex(YELLOW), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_bar, lv_color_hex(BRIGHT_YELLOW), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(co2 >= 2501 && co2 <= 5000) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_container, lv_color_hex(ORANGE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_bar, lv_color_hex(BRIGHT_ORANGE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(co2 >= 5001 && co2 <= 30000) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_container, lv_color_hex(RED), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_bar, lv_color_hex(BRIGHT_RED), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else {
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_container, lv_color_hex(GRAY), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_co2_bar, lv_color_hex(GRAY), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }

    // 处理温度数值
    if(temp >= -400 && temp <= 500)
        sprintf(t,"%.1f",temp/10.0);
    else
        sprintf(t,"----");
    lv_label_set_text(guider_ui.main_screen_temp_value, t);

    // 温度 设置背景颜色
    if(temp >= -400 && temp <= 0 ) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(DARK_BLUE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(BLUE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(temp >= 1 && temp <= 100) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(BLUE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(BRIGHT_BLUE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(temp >= 101 && temp <= 200) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(BRIGHT_BLUE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(BRIGHT_BLUE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(temp >= 201 && temp <= 250) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(DARK_GREEN), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(GREEN), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(temp >= 251 && temp <= 300) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(GREEN), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(BRIGHT_GREEN), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(temp >= 301 && temp <= 350) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(ORANGE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(BRIGHT_ORANGE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(temp >= 351 && temp <= 500) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(RED), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(BRIGHT_RED), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else {
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_container, lv_color_hex(GRAY), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_temp_bar, lv_color_hex(GRAY), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }

    // 处理湿度数值
    if(humid <= 1000)
        sprintf(t,"%.1f",humid/10.0);
    else
        sprintf(t,"----");
    lv_label_set_text(guider_ui.main_screen_humid_value, t);

    // 湿度 设置背景颜色
    if(humid <= 300 ) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_container, lv_color_hex(ORANGE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_bar, lv_color_hex(BRIGHT_ORANGE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(humid >= 301 && humid <= 700) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_container, lv_color_hex(DARK_GREEN), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_bar, lv_color_hex(BRIGHT_GREEN), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else if(humid >= 701 && humid <= 1000) {
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_container, lv_color_hex(DARK_BLUE), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_bar, lv_color_hex(BRIGHT_BLUE), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    else {
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_container, lv_color_hex(GRAY), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.main_screen_humid_bar, lv_color_hex(GRAY), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
}

#include "esp_sntp.h"
#include <time.h>
static void update_time_display() {
    struct tm timeinfo;
    char minute_buf[4], hour_buf[4], month_day_buf[8];
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(hour_buf, sizeof(hour_buf), "%H", &timeinfo);
    strftime(minute_buf, sizeof(minute_buf), "%M", &timeinfo);
    strftime(month_day_buf, sizeof(month_day_buf), "%m/%d", &timeinfo);
    lv_label_set_text(guider_ui.main_screen_time_hour, hour_buf);
    lv_label_set_text(guider_ui.main_screen_time_minute, minute_buf);
    lv_label_set_text(guider_ui.main_screen_time_month_day, month_day_buf);
}

static void update_data_cb(lv_timer_t * timer) {
    update_voc_display();
    update_co2_temp_humid_display();
    update_time_display();
}
#endif


void setup_scr_main_screen(lv_ui *ui)
{
    //Write codes main_screen
    ui->main_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->main_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->main_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_temp_container
    ui->main_screen_temp_container = lv_obj_create(ui->main_screen);
    lv_obj_set_pos(ui->main_screen_temp_container, 18, 40);
    lv_obj_set_size(ui->main_screen_temp_container, 150, 94);
    lv_obj_set_scrollbar_mode(ui->main_screen_temp_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_screen_temp_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_temp_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_temp_container, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_temp_container, 154, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_temp_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_temp_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_temp_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_temp_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_temp_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_temp_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_temp_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_temp_value
    ui->main_screen_temp_value = lv_label_create(ui->main_screen_temp_container);
    lv_obj_set_pos(ui->main_screen_temp_value, 15, 34);
    lv_obj_set_size(ui->main_screen_temp_value, 120, 40);
    lv_label_set_text(ui->main_screen_temp_value, "----");
    lv_label_set_long_mode(ui->main_screen_temp_value, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_temp_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_temp_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_temp_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_temp_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_temp_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_temp_label
    ui->main_screen_temp_label = lv_label_create(ui->main_screen_temp_container);
    lv_obj_set_pos(ui->main_screen_temp_label, 26, 10);
    lv_obj_set_size(ui->main_screen_temp_label, 100, 22);
    lv_label_set_text(ui->main_screen_temp_label, "温度 ℃");
    lv_label_set_long_mode(ui->main_screen_temp_label, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_temp_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_temp_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_temp_label, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_temp_label, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_temp_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_temp_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_temp_bar
    ui->main_screen_temp_bar = lv_bar_create(ui->main_screen_temp_container);
    lv_obj_set_pos(ui->main_screen_temp_bar, 24, 77);
    lv_obj_set_size(ui->main_screen_temp_bar, 100, 8);
    lv_obj_set_style_anim_duration(ui->main_screen_temp_bar, 1000, 0);
    lv_bar_set_mode(ui->main_screen_temp_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->main_screen_temp_bar, 0, 100);
    lv_bar_set_value(ui->main_screen_temp_bar, 100, LV_ANIM_OFF);

    //Write style for main_screen_temp_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_temp_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_temp_bar, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_temp_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_temp_bar, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_temp_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_screen_temp_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_temp_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_temp_bar, lv_color_hex(0x5b5b5b), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_temp_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_temp_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes main_screen_co2_container
    ui->main_screen_co2_container = lv_obj_create(ui->main_screen);
    lv_obj_set_pos(ui->main_screen_co2_container, 18, 155);
    lv_obj_set_size(ui->main_screen_co2_container, 150, 94);
    lv_obj_set_scrollbar_mode(ui->main_screen_co2_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_screen_co2_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_co2_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_co2_container, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_co2_container, 154, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_co2_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_co2_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_co2_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_co2_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_co2_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_co2_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_co2_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_co2_value
    ui->main_screen_co2_value = lv_label_create(ui->main_screen_co2_container);
    lv_obj_set_pos(ui->main_screen_co2_value, 15, 34);
    lv_obj_set_size(ui->main_screen_co2_value, 120, 40);
    lv_label_set_text(ui->main_screen_co2_value, "----");
    lv_label_set_long_mode(ui->main_screen_co2_value, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_co2_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_co2_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_co2_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_co2_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_co2_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_co2_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_co2_label
    ui->main_screen_co2_label = lv_label_create(ui->main_screen_co2_container);
    lv_obj_set_pos(ui->main_screen_co2_label, 26, 10);
    lv_obj_set_size(ui->main_screen_co2_label, 100, 19);
    lv_label_set_text(ui->main_screen_co2_label, "CO2 ppm");
    lv_label_set_long_mode(ui->main_screen_co2_label, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_co2_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_co2_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_co2_label, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_co2_label, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_co2_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_co2_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_co2_bar
    ui->main_screen_co2_bar = lv_bar_create(ui->main_screen_co2_container);
    lv_obj_set_pos(ui->main_screen_co2_bar, 24, 77);
    lv_obj_set_size(ui->main_screen_co2_bar, 100, 8);
    lv_obj_set_style_anim_duration(ui->main_screen_co2_bar, 1000, 0);
    lv_bar_set_mode(ui->main_screen_co2_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->main_screen_co2_bar, 0, 100);
    lv_bar_set_value(ui->main_screen_co2_bar, 100, LV_ANIM_OFF);

    //Write style for main_screen_co2_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_co2_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_co2_bar, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_co2_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_co2_bar, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_co2_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_screen_co2_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_co2_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_co2_bar, lv_color_hex(0x5b5b5b), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_co2_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_co2_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes main_screen_humid_container
    ui->main_screen_humid_container = lv_obj_create(ui->main_screen);
    lv_obj_set_pos(ui->main_screen_humid_container, 188, 40);
    lv_obj_set_size(ui->main_screen_humid_container, 150, 94);
    lv_obj_set_scrollbar_mode(ui->main_screen_humid_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_screen_humid_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_humid_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_humid_container, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_humid_container, 154, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_humid_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_humid_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_humid_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_humid_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_humid_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_humid_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_humid_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_humid_value
    ui->main_screen_humid_value = lv_label_create(ui->main_screen_humid_container);
    lv_obj_set_pos(ui->main_screen_humid_value, 15, 34);
    lv_obj_set_size(ui->main_screen_humid_value, 120, 40);
    lv_label_set_text(ui->main_screen_humid_value, "----");
    lv_label_set_long_mode(ui->main_screen_humid_value, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_humid_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_humid_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_humid_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_humid_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_humid_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_humid_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_humid_label
    ui->main_screen_humid_label = lv_label_create(ui->main_screen_humid_container);
    lv_obj_set_pos(ui->main_screen_humid_label, 26, 10);
    lv_obj_set_size(ui->main_screen_humid_label, 100, 18);
    lv_label_set_text(ui->main_screen_humid_label, "湿度 %");
    lv_label_set_long_mode(ui->main_screen_humid_label, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_humid_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_humid_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_humid_label, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_humid_label, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_humid_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_humid_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_humid_bar
    ui->main_screen_humid_bar = lv_bar_create(ui->main_screen_humid_container);
    lv_obj_set_pos(ui->main_screen_humid_bar, 24, 77);
    lv_obj_set_size(ui->main_screen_humid_bar, 100, 8);
    lv_obj_set_style_anim_duration(ui->main_screen_humid_bar, 1000, 0);
    lv_bar_set_mode(ui->main_screen_humid_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->main_screen_humid_bar, 0, 100);
    lv_bar_set_value(ui->main_screen_humid_bar, 100, LV_ANIM_OFF);

    //Write style for main_screen_humid_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_humid_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_humid_bar, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_humid_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_humid_bar, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_humid_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_screen_humid_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_humid_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_humid_bar, lv_color_hex(0x5b5b5b), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_humid_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_humid_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes main_screen_voc_container
    ui->main_screen_voc_container = lv_obj_create(ui->main_screen);
    lv_obj_set_pos(ui->main_screen_voc_container, 188, 155);
    lv_obj_set_size(ui->main_screen_voc_container, 150, 94);
    lv_obj_set_scrollbar_mode(ui->main_screen_voc_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_screen_voc_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_voc_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_voc_container, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_voc_container, 154, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_voc_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_voc_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_voc_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_voc_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_voc_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_voc_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_voc_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_voc_value
    ui->main_screen_voc_value = lv_label_create(ui->main_screen_voc_container);
    lv_obj_set_pos(ui->main_screen_voc_value, 15, 33);
    lv_obj_set_size(ui->main_screen_voc_value, 120, 40);
    lv_label_set_text(ui->main_screen_voc_value, "----");
    lv_label_set_long_mode(ui->main_screen_voc_value, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_voc_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_voc_value, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_voc_value, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_voc_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_voc_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_voc_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_voc_label
    ui->main_screen_voc_label = lv_label_create(ui->main_screen_voc_container);
    lv_obj_set_pos(ui->main_screen_voc_label, 26, 10);
    lv_obj_set_size(ui->main_screen_voc_label, 100, 18);
    lv_label_set_text(ui->main_screen_voc_label, "VOC指数");
    lv_label_set_long_mode(ui->main_screen_voc_label, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_voc_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_voc_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_voc_label, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_voc_label, 174, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_voc_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_voc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_voc_bar
    ui->main_screen_voc_bar = lv_bar_create(ui->main_screen_voc_container);
    lv_obj_set_pos(ui->main_screen_voc_bar, 24, 77);
    lv_obj_set_size(ui->main_screen_voc_bar, 100, 8);
    lv_obj_set_style_anim_duration(ui->main_screen_voc_bar, 1000, 0);
    lv_bar_set_mode(ui->main_screen_voc_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->main_screen_voc_bar, 0, 100);
    lv_bar_set_value(ui->main_screen_voc_bar, 100, LV_ANIM_OFF);

    //Write style for main_screen_voc_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_voc_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_voc_bar, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_voc_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_voc_bar, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_voc_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_screen_voc_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_screen_voc_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_voc_bar, lv_color_hex(0x5b5b5b), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_voc_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_voc_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes main_screen_time_container
    ui->main_screen_time_container = lv_obj_create(ui->main_screen);
    lv_obj_set_pos(ui->main_screen_time_container, 352, 35);
    lv_obj_set_size(ui->main_screen_time_container, 103, 220);
    lv_obj_set_scrollbar_mode(ui->main_screen_time_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for main_screen_time_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_time_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_time_container, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_time_container, 222, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_screen_time_container, lv_color_hex(0x1B1E20), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_screen_time_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_time_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_time_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_time_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_time_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_time_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_time_hour
    ui->main_screen_time_hour = lv_label_create(ui->main_screen_time_container);
    lv_obj_set_pos(ui->main_screen_time_hour, 17, 30);
    lv_obj_set_size(ui->main_screen_time_hour, 65, 37);
    lv_label_set_text(ui->main_screen_time_hour, "20");
    lv_label_set_long_mode(ui->main_screen_time_hour, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_time_hour, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_time_hour, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_time_hour, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_time_hour, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_time_hour, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_time_hour, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_time_minute
    ui->main_screen_time_minute = lv_label_create(ui->main_screen_time_container);
    lv_obj_set_pos(ui->main_screen_time_minute, 17, 130);
    lv_obj_set_size(ui->main_screen_time_minute, 65, 37);
    lv_label_set_text(ui->main_screen_time_minute, "45");
    lv_label_set_long_mode(ui->main_screen_time_minute, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_time_minute, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_time_minute, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_time_minute, &lv_font_JetBrainsMono_Medium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_time_minute, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_time_minute, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_time_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_screen_time_month_day
    ui->main_screen_time_month_day = lv_label_create(ui->main_screen_time_container);
    lv_obj_set_pos(ui->main_screen_time_month_day, 0, 88);
    lv_obj_set_size(ui->main_screen_time_month_day, 100, 32);
    lv_label_set_text(ui->main_screen_time_month_day, "12/31");
    lv_label_set_long_mode(ui->main_screen_time_month_day, LV_LABEL_LONG_WRAP);

    //Write style for main_screen_time_month_day, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_screen_time_month_day, lv_color_hex(0xFF6A00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_screen_time_month_day, &lv_font_JetBrainsMono_Medium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_screen_time_month_day, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_screen_time_month_day, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_screen_time_month_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of main_screen.
#ifndef LV_USE_GUIDER_SIMULATOR
    if (update_data_timer == NULL)
        update_data_timer = lv_timer_create(update_data_cb, 1000, 0);
#endif

    //Update current screen layout.
    lv_obj_update_layout(ui->main_screen);

    //Init events for screen.
    events_init_main_screen(ui);
}
