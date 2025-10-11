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
#include "esp_err.h"
#include "esp_log.h"
#include "power_management.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
extern _lock_t lvgl_api_lock;

TaskHandle_t update_power_setting_screen_task_handle = NULL;
static void update_power_setting_screen_task(void *args)
{
    char power_status[16], charge_status[16];
    while (1)
    {
        if (guider_ui.power_setting_screen_charge_status_label != NULL)
        {
            // 获取当前电源状态
            switch (pwr_sys_status.chg_stat)
            {
            case AW32001_CHG_STAT_NOT_CHARGING:
                snprintf(charge_status, sizeof(charge_status), "未在充电");
                break;
            case AW32001_CHG_STAT_PRE_CHARGE:
                snprintf(charge_status, sizeof(charge_status), "预充电");
                break;
            case AW32001_CHG_STAT_FAST_CHARGE:
                snprintf(charge_status, sizeof(charge_status), "快速充电");
                break;
            case AW32001_CHG_STAT_CHARGE_DONE:
                snprintf(charge_status, sizeof(charge_status), "充电完成");
                break;
            }
            _lock_acquire(&lvgl_api_lock);
            lv_label_set_text(guider_ui.power_setting_screen_charge_status_label, charge_status);
            _lock_release(&lvgl_api_lock);
        }
        if (guider_ui.power_setting_screen_power_status_label != NULL)
        {
            if (pwr_sys_status.pg_stat)
            {
                snprintf(power_status, sizeof(power_status), "USB供电");
            }
            else
            {
                snprintf(power_status, sizeof(power_status), "电池供电");
            }
            _lock_acquire(&lvgl_api_lock);
            lv_label_set_text(guider_ui.power_setting_screen_power_status_label, power_status);
            _lock_release(&lvgl_api_lock);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void create_update_power_setting_screen_task()
{
    if (update_power_setting_screen_task_handle == NULL)
    {
        xTaskCreatePinnedToCoreWithCaps(update_power_setting_screen_task, "update_power_setting_screen_task", 8 * 1024, NULL, 5, &update_power_setting_screen_task_handle, 1, MALLOC_CAP_SPIRAM);
    }
}

void delete_update_power_setting_screen_task()
{
    if (update_power_setting_screen_task_handle != NULL)
    {
        vTaskDeleteWithCaps(update_power_setting_screen_task_handle);
        update_power_setting_screen_task_handle = NULL;
    }
}

#endif


void setup_scr_power_setting_screen(lv_ui *ui)
{
    //Write codes power_setting_screen
    ui->power_setting_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->power_setting_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->power_setting_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for power_setting_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_cont_1
    ui->power_setting_screen_cont_1 = lv_obj_create(ui->power_setting_screen);
    lv_obj_set_pos(ui->power_setting_screen_cont_1, 30, 32);
    lv_obj_set_size(ui->power_setting_screen_cont_1, 420, 180);
    lv_obj_set_scrollbar_mode(ui->power_setting_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for power_setting_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_cont_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_cont_1, 101, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_cont_1, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_label_3
    ui->power_setting_screen_label_3 = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_label_3, 10, 10);
    lv_obj_set_size(ui->power_setting_screen_label_3, 80, 20);
    lv_label_set_text(ui->power_setting_screen_label_3, "电源管理");
    lv_label_set_long_mode(ui->power_setting_screen_label_3, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_label_3, lv_color_hex(0x26c961), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_label_3, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_pwr_save_mode_label
    ui->power_setting_screen_pwr_save_mode_label = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_pwr_save_mode_label, 10, 40);
    lv_obj_set_size(ui->power_setting_screen_pwr_save_mode_label, 80, 20);
    lv_label_set_text(ui->power_setting_screen_pwr_save_mode_label, "省电模式");
    lv_label_set_long_mode(ui->power_setting_screen_pwr_save_mode_label, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_pwr_save_mode_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_pwr_save_mode_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_pwr_save_mode_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_pwr_save_mode_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_pwr_save_mode_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_pwr_save_mode_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_pwr_save_mode_sw
    ui->power_setting_screen_pwr_save_mode_sw = lv_switch_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_pwr_save_mode_sw, 120, 40);
    lv_obj_set_size(ui->power_setting_screen_pwr_save_mode_sw, 40, 20);

    //Write style for power_setting_screen_pwr_save_mode_sw, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_pwr_save_mode_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_pwr_save_mode_sw, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_pwr_save_mode_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->power_setting_screen_pwr_save_mode_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_pwr_save_mode_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_pwr_save_mode_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for power_setting_screen_pwr_save_mode_sw, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_pwr_save_mode_sw, 207, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->power_setting_screen_pwr_save_mode_sw, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_pwr_save_mode_sw, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->power_setting_screen_pwr_save_mode_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for power_setting_screen_pwr_save_mode_sw, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_pwr_save_mode_sw, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_pwr_save_mode_sw, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_pwr_save_mode_sw, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->power_setting_screen_pwr_save_mode_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_pwr_save_mode_sw, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_fast_charge_sw
    ui->power_setting_screen_fast_charge_sw = lv_switch_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_fast_charge_sw, 330, 40);
    lv_obj_set_size(ui->power_setting_screen_fast_charge_sw, 40, 20);

    //Write style for power_setting_screen_fast_charge_sw, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_fast_charge_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_fast_charge_sw, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_fast_charge_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->power_setting_screen_fast_charge_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_fast_charge_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_fast_charge_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for power_setting_screen_fast_charge_sw, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_fast_charge_sw, 207, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->power_setting_screen_fast_charge_sw, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_fast_charge_sw, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->power_setting_screen_fast_charge_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for power_setting_screen_fast_charge_sw, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_fast_charge_sw, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_fast_charge_sw, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_fast_charge_sw, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->power_setting_screen_fast_charge_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_fast_charge_sw, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_fast_charge_label
    ui->power_setting_screen_fast_charge_label = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_fast_charge_label, 220, 40);
    lv_obj_set_size(ui->power_setting_screen_fast_charge_label, 75, 20);
    lv_label_set_text(ui->power_setting_screen_fast_charge_label, "快速充电");
    lv_label_set_long_mode(ui->power_setting_screen_fast_charge_label, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_fast_charge_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_fast_charge_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_fast_charge_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_fast_charge_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_fast_charge_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_fast_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_charge_thresh_label
    ui->power_setting_screen_charge_thresh_label = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_charge_thresh_label, 10, 80);
    lv_obj_set_size(ui->power_setting_screen_charge_thresh_label, 80, 20);
    lv_label_set_text(ui->power_setting_screen_charge_thresh_label, "充电阈值");
    lv_label_set_long_mode(ui->power_setting_screen_charge_thresh_label, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_charge_thresh_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_charge_thresh_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_charge_thresh_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_charge_thresh_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_charge_thresh_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_charge_thresh_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_charge_thresh_slider
    ui->power_setting_screen_charge_thresh_slider = lv_slider_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_charge_thresh_slider, 109, 84);
    lv_obj_set_size(ui->power_setting_screen_charge_thresh_slider, 216, 10);
    lv_obj_add_flag(ui->power_setting_screen_charge_thresh_slider, LV_OBJ_FLAG_ADV_HITTEST);
    lv_slider_set_range(ui->power_setting_screen_charge_thresh_slider, 70, 100);
    lv_slider_set_mode(ui->power_setting_screen_charge_thresh_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->power_setting_screen_charge_thresh_slider, 80, LV_ANIM_OFF);

    //Write style for power_setting_screen_charge_thresh_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_charge_thresh_slider, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_charge_thresh_slider, lv_color_hex(0x34ffa4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_charge_thresh_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_charge_thresh_slider, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->power_setting_screen_charge_thresh_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_charge_thresh_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for power_setting_screen_charge_thresh_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_charge_thresh_slider, 201, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_charge_thresh_slider, lv_color_hex(0x34ff98), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_charge_thresh_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_charge_thresh_slider, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for power_setting_screen_charge_thresh_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_charge_thresh_slider, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_charge_thresh_slider, lv_color_hex(0x00ed5c), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_charge_thresh_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_charge_thresh_slider, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_charge_thresh_percent_label
    ui->power_setting_screen_charge_thresh_percent_label = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_charge_thresh_percent_label, 340, 80);
    lv_obj_set_size(ui->power_setting_screen_charge_thresh_percent_label, 69, 20);
    lv_label_set_text(ui->power_setting_screen_charge_thresh_percent_label, "--%");
    lv_label_set_long_mode(ui->power_setting_screen_charge_thresh_percent_label, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_charge_thresh_percent_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_charge_thresh_percent_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_charge_thresh_percent_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_charge_thresh_percent_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_charge_thresh_percent_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_charge_thresh_percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_label_1
    ui->power_setting_screen_label_1 = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_label_1, 9, 120);
    lv_obj_set_size(ui->power_setting_screen_label_1, 80, 20);
    lv_label_set_text(ui->power_setting_screen_label_1, "充电状态");
    lv_label_set_long_mode(ui->power_setting_screen_label_1, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_label_1, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_charge_status_label
    ui->power_setting_screen_charge_status_label = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_charge_status_label, 110, 120);
    lv_obj_set_size(ui->power_setting_screen_charge_status_label, 71, 20);
    lv_label_set_text(ui->power_setting_screen_charge_status_label, "未在充电\n预充电\n快速充电\n充电完成");
    lv_label_set_long_mode(ui->power_setting_screen_charge_status_label, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_charge_status_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_charge_status_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_charge_status_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_charge_status_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_charge_status_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_charge_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_label_5
    ui->power_setting_screen_label_5 = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_label_5, 220, 120);
    lv_obj_set_size(ui->power_setting_screen_label_5, 71, 20);
    lv_label_set_text(ui->power_setting_screen_label_5, "供电状态");
    lv_label_set_long_mode(ui->power_setting_screen_label_5, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_label_5, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_label_5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_power_status_label
    ui->power_setting_screen_power_status_label = lv_label_create(ui->power_setting_screen_cont_1);
    lv_obj_set_pos(ui->power_setting_screen_power_status_label, 310, 120);
    lv_obj_set_size(ui->power_setting_screen_power_status_label, 71, 20);
    lv_label_set_text(ui->power_setting_screen_power_status_label, "USB供电\n电池供电");
    lv_label_set_long_mode(ui->power_setting_screen_power_status_label, LV_LABEL_LONG_WRAP);

    //Write style for power_setting_screen_power_status_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_power_status_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_power_status_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_power_status_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_power_status_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_power_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_save_btn
    ui->power_setting_screen_save_btn = lv_button_create(ui->power_setting_screen);
    lv_obj_set_pos(ui->power_setting_screen_save_btn, 280, 230);
    lv_obj_set_size(ui->power_setting_screen_save_btn, 120, 30);
    ui->power_setting_screen_save_btn_label = lv_label_create(ui->power_setting_screen_save_btn);
    lv_label_set_text(ui->power_setting_screen_save_btn_label, "保存");
    lv_label_set_long_mode(ui->power_setting_screen_save_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->power_setting_screen_save_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->power_setting_screen_save_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->power_setting_screen_save_btn_label, LV_PCT(100));

    //Write style for power_setting_screen_save_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_save_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_save_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_save_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->power_setting_screen_save_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_save_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_save_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_save_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_save_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_save_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_save_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes power_setting_screen_return_btn
    ui->power_setting_screen_return_btn = lv_button_create(ui->power_setting_screen);
    lv_obj_set_pos(ui->power_setting_screen_return_btn, 80, 230);
    lv_obj_set_size(ui->power_setting_screen_return_btn, 120, 30);
    ui->power_setting_screen_return_btn_label = lv_label_create(ui->power_setting_screen_return_btn);
    lv_label_set_text(ui->power_setting_screen_return_btn_label, "返回");
    lv_label_set_long_mode(ui->power_setting_screen_return_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->power_setting_screen_return_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->power_setting_screen_return_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->power_setting_screen_return_btn_label, LV_PCT(100));

    //Write style for power_setting_screen_return_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->power_setting_screen_return_btn, 109, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->power_setting_screen_return_btn, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->power_setting_screen_return_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->power_setting_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->power_setting_screen_return_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->power_setting_screen_return_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->power_setting_screen_return_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->power_setting_screen_return_btn, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->power_setting_screen_return_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->power_setting_screen_return_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of power_setting_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->power_setting_screen);

    //Init events for screen.
    events_init_power_setting_screen(ui);
}
