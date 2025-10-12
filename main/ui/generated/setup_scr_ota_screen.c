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
#include "ota.h"
#include "nvs_helper.h"
extern _lock_t lvgl_api_lock;
TaskHandle_t update_ota_screen_task_handle = NULL;
static void update_ota_screen_task(void *args)
{
    uint8_t old_ota_status = OTA_STATUS_IDLE;
    char t[36];
    char *ota_version = malloc(32 * sizeof(char));
    nvs_read(NVS_READ_OTA);
    ota_get_current_version(ota_version);
    snprintf(t, sizeof(t), "v%s", ota_version);
    _lock_acquire(&lvgl_api_lock);
    lv_label_set_text(guider_ui.ota_screen_current_version_label, t);
    lv_textarea_set_text(guider_ui.ota_screen_url_input, ota_settings.info_url); // 显示保存的URL
    lv_label_set_text(guider_ui.ota_screen_hint_label, "点击下方按钮检查更新");
    lv_obj_add_flag(guider_ui.ota_screen_newest_version_hint_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.ota_screen_newest_version_label, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(guider_ui.ota_screen_update_btn_label, "检查更新");
    lv_obj_add_flag(guider_ui.ota_screen_progress_bar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.ota_screen_progress_label, LV_OBJ_FLAG_HIDDEN);
    lv_bar_set_value(guider_ui.ota_screen_progress_bar, 0, LV_ANIM_OFF);
    lv_label_set_text(guider_ui.ota_screen_progress_label, "0.00%");
    _lock_release(&lvgl_api_lock);
    free(ota_version);

    while (1)
    {
        if (old_ota_status != ota_status)
        {
            old_ota_status = ota_status;

            _lock_acquire(&lvgl_api_lock);
            // 根据ota_status更新提示标签
            switch (ota_status)
            {
            case OTA_STATUS_IDLE:
                lv_label_set_text(guider_ui.ota_screen_hint_label, "点击下方按钮检查更新");
                break;
            case OTA_STATUS_CHECKING_UPDATE:
                lv_label_set_text(guider_ui.ota_screen_hint_label, "检查更新中");
                break;
            case OTA_STATUS_DOWNLOADING:
                lv_label_set_text(guider_ui.ota_screen_hint_label, "更新中，请勿关闭电源");
                break;
            case OTA_STATUS_FAILED:
                lv_label_set_text(guider_ui.ota_screen_hint_label, "更新失败");
                break;
            case OTA_STATUS_NO_UPDATE:
                lv_label_set_text(guider_ui.ota_screen_hint_label, "当前已是最新版本");
                break;
            case OTA_STATUS_FOUND_UPDATE:
                lv_label_set_text(guider_ui.ota_screen_hint_label, "发现新版本");
                break;
            }
            // 版本号
            switch (ota_status)
            {
            case OTA_STATUS_IDLE:
            case OTA_STATUS_CHECKING_UPDATE:
            case OTA_STATUS_NO_UPDATE:
            case OTA_STATUS_FAILED:
                snprintf(t, sizeof(t), "?");
                lv_label_set_text(guider_ui.ota_screen_newest_version_label, t);
                lv_obj_add_flag(guider_ui.ota_screen_newest_version_hint_label, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.ota_screen_newest_version_label, LV_OBJ_FLAG_HIDDEN);
                break;
            case OTA_STATUS_FOUND_UPDATE:
            case OTA_STATUS_DOWNLOADING:
                snprintf(t, sizeof(t), "v%s", ota_settings.newest_version);
                lv_label_set_text(guider_ui.ota_screen_newest_version_label, t);
                lv_obj_clear_flag(guider_ui.ota_screen_newest_version_hint_label, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(guider_ui.ota_screen_newest_version_label, LV_OBJ_FLAG_HIDDEN);
                break;
            }
            // 底部按钮
            switch (ota_status)
            {
            case OTA_STATUS_DOWNLOADING:
                lv_obj_add_flag(guider_ui.ota_screen_update_btn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.ota_screen_return_btn, LV_OBJ_FLAG_HIDDEN);
                break;
            case OTA_STATUS_IDLE:
            case OTA_STATUS_NO_UPDATE:
            case OTA_STATUS_FAILED:
            case OTA_STATUS_CHECKING_UPDATE:
                lv_obj_clear_flag(guider_ui.ota_screen_return_btn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(guider_ui.ota_screen_update_btn, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(guider_ui.ota_screen_update_btn_label, "检查更新");
                break;
            case OTA_STATUS_FOUND_UPDATE:
                lv_obj_clear_flag(guider_ui.ota_screen_return_btn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(guider_ui.ota_screen_update_btn, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(guider_ui.ota_screen_update_btn_label, "立即更新");
                break;
            }
            // 进度条
            switch (ota_status)
            {
            case OTA_STATUS_DOWNLOADING:
                lv_obj_clear_flag(guider_ui.ota_screen_progress_bar, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(guider_ui.ota_screen_progress_label, LV_OBJ_FLAG_HIDDEN);
                lv_bar_set_value(guider_ui.ota_screen_progress_bar, ota_get_download_progress(), LV_ANIM_ON);
                break;
            default:
                lv_obj_add_flag(guider_ui.ota_screen_progress_bar, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.ota_screen_progress_label, LV_OBJ_FLAG_HIDDEN);
                lv_bar_set_value(guider_ui.ota_screen_progress_bar, 0, LV_ANIM_OFF);
                lv_label_set_text(guider_ui.ota_screen_progress_label, "0.00%");
                break;
            }
            _lock_release(&lvgl_api_lock);
        }
        else if (ota_status == OTA_STATUS_DOWNLOADING)
        {
            char progress[8];
            float ota_progress = ota_get_download_progress();
            // 更新进度条
            _lock_acquire(&lvgl_api_lock);
            lv_bar_set_value(guider_ui.ota_screen_progress_bar, (int32_t)ota_progress, LV_ANIM_ON);
            snprintf(progress, sizeof(progress), "%.2f%%", ota_progress);
            lv_label_set_text(guider_ui.ota_screen_progress_label, progress);
            _lock_release(&lvgl_api_lock);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void create_update_ota_screen_task()
{
    if (update_ota_screen_task_handle == NULL)
    {
        xTaskCreate(update_ota_screen_task, "update_ota_screen_task", 8 * 1024, NULL, 5, &update_ota_screen_task_handle);
    }
}

void delete_update_ota_screen_task()
{
    if (update_ota_screen_task_handle != NULL)
    {
        vTaskDelete(update_ota_screen_task_handle);
        update_ota_screen_task_handle = NULL;
    }
}
#endif


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

    //Write codes ota_screen_ota_container
    ui->ota_screen_ota_container = lv_obj_create(ui->ota_screen);
    lv_obj_set_pos(ui->ota_screen_ota_container, 30, 39);
    lv_obj_set_size(ui->ota_screen_ota_container, 420, 180);
    lv_obj_set_scrollbar_mode(ui->ota_screen_ota_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for ota_screen_ota_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_ota_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_ota_container, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_ota_container, 101, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_ota_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_ota_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_ota_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_ota_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_ota_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_ota_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_ota_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_1
    ui->ota_screen_label_1 = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_label_1, 17, 7);
    lv_obj_set_size(ui->ota_screen_label_1, 384, 26);
    lv_label_set_text(ui->ota_screen_label_1, "OTA");
    lv_label_set_long_mode(ui->ota_screen_label_1, LV_LABEL_LONG_SCROLL_CIRCULAR);

    //Write style for ota_screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_1, &lv_font_JetBrainsMono_Medium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_progress_bar
    ui->ota_screen_progress_bar = lv_bar_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_progress_bar, 60, 158);
    lv_obj_set_size(ui->ota_screen_progress_bar, 300, 10);
    lv_obj_set_style_anim_duration(ui->ota_screen_progress_bar, 1000, 0);
    lv_bar_set_mode(ui->ota_screen_progress_bar, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->ota_screen_progress_bar, 0, 100);
    lv_bar_set_value(ui->ota_screen_progress_bar, 0, LV_ANIM_ON);

    //Write style for ota_screen_progress_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_progress_bar, 53, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_progress_bar, lv_color_hex(0x1a7f14), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_progress_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_progress_bar, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_progress_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ota_screen_progress_bar, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_progress_bar, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_progress_bar, lv_color_hex(0x26c961), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_progress_bar, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_progress_bar, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes ota_screen_url_input
    ui->ota_screen_url_input = lv_textarea_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_url_input, 87, 44);
    lv_obj_set_size(ui->ota_screen_url_input, 259, 36);
    lv_textarea_set_text(ui->ota_screen_url_input, "");
    lv_textarea_set_placeholder_text(ui->ota_screen_url_input, "URL");
    lv_textarea_set_password_bullet(ui->ota_screen_url_input, "*");
    lv_textarea_set_password_mode(ui->ota_screen_url_input, false);
    lv_textarea_set_one_line(ui->ota_screen_url_input, true);
    lv_textarea_set_accepted_chars(ui->ota_screen_url_input, "");
    lv_textarea_set_max_length(ui->ota_screen_url_input, 128);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->ota_screen_url_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for ota_screen_url_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->ota_screen_url_input, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_url_input, &lv_font_siyuanheiti_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_url_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_url_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_url_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_url_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_url_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_url_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ota_screen_url_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_url_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_url_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_url_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_url_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_url_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for ota_screen_url_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_url_input, 104, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_url_input, lv_color_hex(0xffffff), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_url_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_url_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes ota_screen_label_2
    ui->ota_screen_label_2 = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_label_2, 16, 52);
    lv_obj_set_size(ui->ota_screen_label_2, 59, 20);
    lv_label_set_text(ui->ota_screen_label_2, "更新源");
    lv_label_set_long_mode(ui->ota_screen_label_2, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_label_2, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_current_version_hint_label
    ui->ota_screen_current_version_hint_label = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_current_version_hint_label, 16, 94);
    lv_obj_set_size(ui->ota_screen_current_version_hint_label, 74, 20);
    lv_label_set_text(ui->ota_screen_current_version_hint_label, "当前版本");
    lv_label_set_long_mode(ui->ota_screen_current_version_hint_label, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_current_version_hint_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_current_version_hint_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_current_version_hint_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_current_version_hint_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_current_version_hint_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_current_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_current_version_label
    ui->ota_screen_current_version_label = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_current_version_label, 96, 94);
    lv_obj_set_size(ui->ota_screen_current_version_label, 105, 20);
    lv_label_set_text(ui->ota_screen_current_version_label, "v1.x.x");
    lv_label_set_long_mode(ui->ota_screen_current_version_label, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_current_version_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_current_version_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_current_version_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_current_version_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_current_version_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_current_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_newest_version_hint_label
    ui->ota_screen_newest_version_hint_label = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_newest_version_hint_label, 206, 94);
    lv_obj_set_size(ui->ota_screen_newest_version_hint_label, 74, 20);
    lv_label_set_text(ui->ota_screen_newest_version_hint_label, "最新版本");
    lv_label_set_long_mode(ui->ota_screen_newest_version_hint_label, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_newest_version_hint_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_newest_version_hint_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_newest_version_hint_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_newest_version_hint_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_newest_version_hint_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_newest_version_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_newest_version_label
    ui->ota_screen_newest_version_label = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_newest_version_label, 286, 94);
    lv_obj_set_size(ui->ota_screen_newest_version_label, 107, 20);
    lv_label_set_text(ui->ota_screen_newest_version_label, "v1.x.x");
    lv_label_set_long_mode(ui->ota_screen_newest_version_label, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_newest_version_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_newest_version_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_newest_version_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_newest_version_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_newest_version_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_newest_version_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_hint_label
    ui->ota_screen_hint_label = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_hint_label, 69, 132);
    lv_obj_set_size(ui->ota_screen_hint_label, 282, 20);
    lv_label_set_text(ui->ota_screen_hint_label, "更新中，请勿关闭电源\n更新失败\n网络未连接\n当前已是最新版本\n发现新版本\n检查更新中\n点击下方按钮检查更新");
    lv_label_set_long_mode(ui->ota_screen_hint_label, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_hint_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_hint_label, lv_color_hex(0xff7300), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_hint_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_hint_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_hint_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_hint_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes ota_screen_progress_label
    ui->ota_screen_progress_label = lv_label_create(ui->ota_screen_ota_container);
    lv_obj_set_pos(ui->ota_screen_progress_label, 180, 155);
    lv_obj_set_size(ui->ota_screen_progress_label, 70, 20);
    lv_label_set_text(ui->ota_screen_progress_label, "0.00%");
    lv_label_set_long_mode(ui->ota_screen_progress_label, LV_LABEL_LONG_WRAP);

    //Write style for ota_screen_progress_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_progress_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_progress_label, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_progress_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_progress_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_progress_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

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
    lv_label_set_text(ui->ota_screen_update_btn_label, "检查更新\n立即更新\n正在更新");
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

    //Write codes ota_screen_btn_1
    ui->ota_screen_btn_1 = lv_button_create(ui->ota_screen);
    lv_obj_set_pos(ui->ota_screen_btn_1, 383, 84);
    lv_obj_set_size(ui->ota_screen_btn_1, 58, 35);
    ui->ota_screen_btn_1_label = lv_label_create(ui->ota_screen_btn_1);
    lv_label_set_text(ui->ota_screen_btn_1_label, "重置");
    lv_label_set_long_mode(ui->ota_screen_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->ota_screen_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->ota_screen_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->ota_screen_btn_1_label, LV_PCT(100));

    //Write style for ota_screen_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->ota_screen_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ota_screen_btn_1, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->ota_screen_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ota_screen_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ota_screen_btn_1, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->ota_screen_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->ota_screen_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->ota_screen_btn_1, &lv_font_siyuanheiti_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->ota_screen_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->ota_screen_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of ota_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->ota_screen);

    //Init events for screen.
    events_init_ota_screen(ui);
}
