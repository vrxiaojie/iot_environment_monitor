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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_user.h"
extern _lock_t lvgl_api_lock;
TaskHandle_t update_mqtt_screen_task_handle = NULL;
static void update_mqtt_screen_task(void *args)
{
    while (1)
    {
        _lock_acquire(&lvgl_api_lock);
        if (mqtt_get_status() == 1)
        {
            lv_label_set_text(guider_ui.mqtt_setting_screen_connect_btn_label, "断开");
            lv_label_set_text(guider_ui.mqtt_setting_screen_mqtt_status_label, "已连接");
            lv_obj_set_style_text_color(guider_ui.mqtt_setting_screen_mqtt_status_label, lv_color_hex(0x26c961), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else if (mqtt_get_status() == 2)
        {
            lv_label_set_text(guider_ui.mqtt_setting_screen_connect_btn_label, "保存&连接");
            lv_label_set_text(guider_ui.mqtt_setting_screen_mqtt_status_label, "连接失败");
            lv_obj_set_style_text_color(guider_ui.mqtt_setting_screen_mqtt_status_label, lv_color_hex(0xE8202D), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_label_set_text(guider_ui.mqtt_setting_screen_connect_btn_label, "保存&连接");
            lv_label_set_text(guider_ui.mqtt_setting_screen_mqtt_status_label, "未连接");
            lv_obj_set_style_text_color(guider_ui.mqtt_setting_screen_mqtt_status_label, lv_color_hex(0xdeef18), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        _lock_release(&lvgl_api_lock);
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));
    }
}
void create_update_mqtt_screen_task()
{
    if (update_mqtt_screen_task_handle == NULL)
    {
        xTaskCreatePinnedToCoreWithCaps(update_mqtt_screen_task, "update_mqtt_screen_task", 8 * 1024, NULL, 5, &update_mqtt_screen_task_handle, 1, MALLOC_CAP_SPIRAM);
    }
}

void delete_update_mqtt_screen_task()
{
    if (update_mqtt_screen_task_handle != NULL)
    {
        vTaskDeleteWithCaps(update_mqtt_screen_task_handle);
        update_mqtt_screen_task_handle = NULL;
    }
}
#endif


void setup_scr_mqtt_setting_screen(lv_ui *ui)
{
    //Write codes mqtt_setting_screen
    ui->mqtt_setting_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->mqtt_setting_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->mqtt_setting_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for mqtt_setting_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_cont_1
    ui->mqtt_setting_screen_cont_1 = lv_obj_create(ui->mqtt_setting_screen);
    lv_obj_set_pos(ui->mqtt_setting_screen_cont_1, 30, 32);
    lv_obj_set_size(ui->mqtt_setting_screen_cont_1, 420, 180);
    lv_obj_set_scrollbar_mode(ui->mqtt_setting_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for mqtt_setting_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_cont_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_cont_1, 101, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_cont_1, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_14
    ui->mqtt_setting_screen_label_14 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_14, 10, 10);
    lv_obj_set_size(ui->mqtt_setting_screen_label_14, 100, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_14, "MQTT设置");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_14, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_14, lv_color_hex(0x26c961), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_14, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_14, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_13
    ui->mqtt_setting_screen_label_13 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_13, 20, 134);
    lv_obj_set_size(ui->mqtt_setting_screen_label_13, 80, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_13, "自动连接");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_13, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_13, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_13, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_auto_connect_switch
    ui->mqtt_setting_screen_auto_connect_switch = lv_switch_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_auto_connect_switch, 112, 134);
    lv_obj_set_size(ui->mqtt_setting_screen_auto_connect_switch, 40, 20);

    //Write style for mqtt_setting_screen_auto_connect_switch, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_auto_connect_switch, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_auto_connect_switch, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_auto_connect_switch, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_auto_connect_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_auto_connect_switch, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_auto_connect_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for mqtt_setting_screen_auto_connect_switch, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_auto_connect_switch, 207, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_auto_connect_switch, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_auto_connect_switch, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_auto_connect_switch, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for mqtt_setting_screen_auto_connect_switch, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_auto_connect_switch, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_auto_connect_switch, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_auto_connect_switch, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_auto_connect_switch, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_auto_connect_switch, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_11
    ui->mqtt_setting_screen_label_11 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_11, 200, 134);
    lv_obj_set_size(ui->mqtt_setting_screen_label_11, 80, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_11, "上传间隔");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_11, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_11, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_11, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_7
    ui->mqtt_setting_screen_label_7 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_7, 150, 10);
    lv_obj_set_size(ui->mqtt_setting_screen_label_7, 71, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_7, "当前状态");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_7, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_7, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_7, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_mqtt_status_label
    ui->mqtt_setting_screen_mqtt_status_label = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_mqtt_status_label, 240, 10);
    lv_obj_set_size(ui->mqtt_setting_screen_mqtt_status_label, 71, 20);
    lv_label_set_text(ui->mqtt_setting_screen_mqtt_status_label, "已连接\n未连接\n连接失败");
    lv_label_set_long_mode(ui->mqtt_setting_screen_mqtt_status_label, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_mqtt_status_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_mqtt_status_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_mqtt_status_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_mqtt_status_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_mqtt_status_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_mqtt_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_15
    ui->mqtt_setting_screen_label_15 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_15, 20, 44);
    lv_obj_set_size(ui->mqtt_setting_screen_label_15, 37, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_15, "地址");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_15, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_15, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_15, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_upload_interval_list
    ui->mqtt_setting_screen_upload_interval_list = lv_dropdown_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_upload_interval_list, 300, 129);
    lv_obj_set_size(ui->mqtt_setting_screen_upload_interval_list, 62, 28);
    lv_dropdown_set_options(ui->mqtt_setting_screen_upload_interval_list, "5s\n30s\n60s");

    //Write style for mqtt_setting_screen_upload_interval_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_upload_interval_list, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_upload_interval_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_upload_interval_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_upload_interval_list, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_upload_interval_list, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_upload_interval_list, 129, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_upload_interval_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_upload_interval_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for mqtt_setting_screen_upload_interval_list, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_upload_interval_list, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_upload_interval_list, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_upload_interval_list, 1, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(ui->mqtt_setting_screen_upload_interval_list, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_border_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_border_side(ui->mqtt_setting_screen_upload_interval_list, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_upload_interval_list, 8, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_upload_interval_list, 3, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_upload_interval_list, 128, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_upload_interval_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_upload_interval_list, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for mqtt_setting_screen_upload_interval_list, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_upload_interval_list, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_upload_interval_list, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_upload_interval_list, 0, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_upload_interval_list, 8, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_upload_interval_list, 3, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_upload_interval_list, 130, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_upload_interval_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_upload_interval_list, 0, LV_PART_MAIN|LV_STATE_FOCUSED);

    //Write style for mqtt_setting_screen_upload_interval_list, Part: LV_PART_MAIN, State: LV_STATE_DISABLED.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_upload_interval_list, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_upload_interval_list, 255, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_upload_interval_list, 0, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_upload_interval_list, 8, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_upload_interval_list, 6, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_upload_interval_list, 3, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_upload_interval_list, 128, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_upload_interval_list, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_upload_interval_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DISABLED);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_upload_interval_list, 0, LV_PART_MAIN|LV_STATE_DISABLED);

    //Write style state: LV_STATE_CHECKED for &style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked
    static lv_style_t style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked;
    ui_init_style(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked);

    lv_style_set_border_width(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, lv_color_hex(0x0f4187));
    lv_style_set_border_side(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, 104);
    lv_style_set_bg_color(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, lv_color_hex(0x000000));
    lv_style_set_bg_grad_dir(&style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->mqtt_setting_screen_upload_interval_list), &style_mqtt_setting_screen_upload_interval_list_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_mqtt_setting_screen_upload_interval_list_extra_list_main_default
    static lv_style_t style_mqtt_setting_screen_upload_interval_list_extra_list_main_default;
    ui_init_style(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default);

    lv_style_set_max_height(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, 90);
    lv_style_set_text_color(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, &lv_font_JetBrainsMono_Medium_12);
    lv_style_set_text_opa(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, 255);
    lv_style_set_border_width(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, 0);
    lv_style_set_radius(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, 4);
    lv_style_set_bg_opa(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, 129);
    lv_style_set_bg_color(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, lv_color_hex(0x5b5b5b));
    lv_style_set_bg_grad_dir(&style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->mqtt_setting_screen_upload_interval_list), &style_mqtt_setting_screen_upload_interval_list_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default
    static lv_style_t style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default;
    ui_init_style(&style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default);

    lv_style_set_radius(&style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->mqtt_setting_screen_upload_interval_list), &style_mqtt_setting_screen_upload_interval_list_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_16
    ui->mqtt_setting_screen_label_16 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_16, 300, 44);
    lv_obj_set_size(ui->mqtt_setting_screen_label_16, 37, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_16, "端口");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_16, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_16, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_16, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_port_input
    ui->mqtt_setting_screen_port_input = lv_textarea_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_port_input, 340, 35);
    lv_obj_set_size(ui->mqtt_setting_screen_port_input, 53, 36);
    lv_textarea_set_text(ui->mqtt_setting_screen_port_input, "1883");
    lv_textarea_set_placeholder_text(ui->mqtt_setting_screen_port_input, "Port");
    lv_textarea_set_password_bullet(ui->mqtt_setting_screen_port_input, "*");
    lv_textarea_set_password_mode(ui->mqtt_setting_screen_port_input, false);
    lv_textarea_set_one_line(ui->mqtt_setting_screen_port_input, true);
    lv_textarea_set_accepted_chars(ui->mqtt_setting_screen_port_input, "0123456789");
    lv_textarea_set_max_length(ui->mqtt_setting_screen_port_input, 5);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->mqtt_setting_screen_port_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for mqtt_setting_screen_port_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_port_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_port_input, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_port_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_port_input, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_port_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_port_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_port_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_port_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_port_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_port_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_port_input, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_port_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_port_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_port_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for mqtt_setting_screen_port_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_port_input, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_port_input, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_port_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_port_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_address_input
    ui->mqtt_setting_screen_address_input = lv_textarea_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_address_input, 80, 35);
    lv_obj_set_size(ui->mqtt_setting_screen_address_input, 212, 36);
    lv_textarea_set_text(ui->mqtt_setting_screen_address_input, "mqtt://");
    lv_textarea_set_placeholder_text(ui->mqtt_setting_screen_address_input, "Broker uri e.g mqtt://10.0.0.1");
    lv_textarea_set_password_bullet(ui->mqtt_setting_screen_address_input, "*");
    lv_textarea_set_password_mode(ui->mqtt_setting_screen_address_input, false);
    lv_textarea_set_one_line(ui->mqtt_setting_screen_address_input, true);
    lv_textarea_set_accepted_chars(ui->mqtt_setting_screen_address_input, "");
    lv_textarea_set_max_length(ui->mqtt_setting_screen_address_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->mqtt_setting_screen_address_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for mqtt_setting_screen_address_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_address_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_address_input, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_address_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_address_input, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_address_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_address_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_address_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_address_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_address_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_address_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_address_input, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_address_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_address_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_address_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for mqtt_setting_screen_address_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_address_input, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_address_input, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_address_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_address_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_18
    ui->mqtt_setting_screen_label_18 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_18, 20, 90);
    lv_obj_set_size(ui->mqtt_setting_screen_label_18, 49, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_18, "用户名");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_18, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_18, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_18, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_18, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_username_input
    ui->mqtt_setting_screen_username_input = lv_textarea_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_username_input, 80, 82);
    lv_obj_set_size(ui->mqtt_setting_screen_username_input, 128, 36);
    lv_textarea_set_text(ui->mqtt_setting_screen_username_input, "");
    lv_textarea_set_placeholder_text(ui->mqtt_setting_screen_username_input, "Username");
    lv_textarea_set_password_bullet(ui->mqtt_setting_screen_username_input, "*");
    lv_textarea_set_password_mode(ui->mqtt_setting_screen_username_input, false);
    lv_textarea_set_one_line(ui->mqtt_setting_screen_username_input, true);
    lv_textarea_set_accepted_chars(ui->mqtt_setting_screen_username_input, "");
    lv_textarea_set_max_length(ui->mqtt_setting_screen_username_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->mqtt_setting_screen_username_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for mqtt_setting_screen_username_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_username_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_username_input, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_username_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_username_input, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_username_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_username_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_username_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_username_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_username_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_username_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_username_input, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_username_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_username_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_username_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for mqtt_setting_screen_username_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_username_input, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_username_input, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_username_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_username_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_label_17
    ui->mqtt_setting_screen_label_17 = lv_label_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_label_17, 229, 90);
    lv_obj_set_size(ui->mqtt_setting_screen_label_17, 37, 20);
    lv_label_set_text(ui->mqtt_setting_screen_label_17, "密码");
    lv_label_set_long_mode(ui->mqtt_setting_screen_label_17, LV_LABEL_LONG_WRAP);

    //Write style for mqtt_setting_screen_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_label_17, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_label_17, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_label_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_label_17, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_passwd_input
    ui->mqtt_setting_screen_passwd_input = lv_textarea_create(ui->mqtt_setting_screen_cont_1);
    lv_obj_set_pos(ui->mqtt_setting_screen_passwd_input, 280, 82);
    lv_obj_set_size(ui->mqtt_setting_screen_passwd_input, 128, 36);
    lv_textarea_set_text(ui->mqtt_setting_screen_passwd_input, "");
    lv_textarea_set_placeholder_text(ui->mqtt_setting_screen_passwd_input, "Password");
    lv_textarea_set_password_bullet(ui->mqtt_setting_screen_passwd_input, "*");
    lv_textarea_set_password_mode(ui->mqtt_setting_screen_passwd_input, false);
    lv_textarea_set_one_line(ui->mqtt_setting_screen_passwd_input, true);
    lv_textarea_set_accepted_chars(ui->mqtt_setting_screen_passwd_input, "");
    lv_textarea_set_max_length(ui->mqtt_setting_screen_passwd_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->mqtt_setting_screen_passwd_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for mqtt_setting_screen_passwd_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_passwd_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_passwd_input, &lv_font_JetBrainsMono_Medium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_passwd_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->mqtt_setting_screen_passwd_input, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_passwd_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_passwd_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_passwd_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_passwd_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_passwd_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_passwd_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->mqtt_setting_screen_passwd_input, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->mqtt_setting_screen_passwd_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->mqtt_setting_screen_passwd_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_passwd_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for mqtt_setting_screen_passwd_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_passwd_input, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_passwd_input, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_passwd_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_passwd_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_save_btn
    ui->mqtt_setting_screen_save_btn = lv_button_create(ui->mqtt_setting_screen);
    lv_obj_set_pos(ui->mqtt_setting_screen_save_btn, 180, 230);
    lv_obj_set_size(ui->mqtt_setting_screen_save_btn, 120, 30);
    ui->mqtt_setting_screen_save_btn_label = lv_label_create(ui->mqtt_setting_screen_save_btn);
    lv_label_set_text(ui->mqtt_setting_screen_save_btn_label, "保存");
    lv_label_set_long_mode(ui->mqtt_setting_screen_save_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->mqtt_setting_screen_save_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->mqtt_setting_screen_save_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->mqtt_setting_screen_save_btn_label, LV_PCT(100));

    //Write style for mqtt_setting_screen_save_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_save_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_save_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_save_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_save_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_save_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_save_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_save_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_save_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_save_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_save_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_return_btn
    ui->mqtt_setting_screen_return_btn = lv_button_create(ui->mqtt_setting_screen);
    lv_obj_set_pos(ui->mqtt_setting_screen_return_btn, 30, 230);
    lv_obj_set_size(ui->mqtt_setting_screen_return_btn, 120, 30);
    ui->mqtt_setting_screen_return_btn_label = lv_label_create(ui->mqtt_setting_screen_return_btn);
    lv_label_set_text(ui->mqtt_setting_screen_return_btn_label, "返回");
    lv_label_set_long_mode(ui->mqtt_setting_screen_return_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->mqtt_setting_screen_return_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->mqtt_setting_screen_return_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->mqtt_setting_screen_return_btn_label, LV_PCT(100));

    //Write style for mqtt_setting_screen_return_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_return_btn, 109, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_return_btn, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_return_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_return_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_return_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_return_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_return_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_return_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes mqtt_setting_screen_connect_btn
    ui->mqtt_setting_screen_connect_btn = lv_button_create(ui->mqtt_setting_screen);
    lv_obj_set_pos(ui->mqtt_setting_screen_connect_btn, 330, 230);
    lv_obj_set_size(ui->mqtt_setting_screen_connect_btn, 120, 30);
    ui->mqtt_setting_screen_connect_btn_label = lv_label_create(ui->mqtt_setting_screen_connect_btn);
    lv_label_set_text(ui->mqtt_setting_screen_connect_btn_label, "保存&连接\n断开");
    lv_label_set_long_mode(ui->mqtt_setting_screen_connect_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->mqtt_setting_screen_connect_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->mqtt_setting_screen_connect_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->mqtt_setting_screen_connect_btn_label, LV_PCT(100));

    //Write style for mqtt_setting_screen_connect_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->mqtt_setting_screen_connect_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->mqtt_setting_screen_connect_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->mqtt_setting_screen_connect_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->mqtt_setting_screen_connect_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->mqtt_setting_screen_connect_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->mqtt_setting_screen_connect_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->mqtt_setting_screen_connect_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->mqtt_setting_screen_connect_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->mqtt_setting_screen_connect_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->mqtt_setting_screen_connect_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of mqtt_setting_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->mqtt_setting_screen);

    //Init events for screen.
    events_init_mqtt_setting_screen(ui);
}
