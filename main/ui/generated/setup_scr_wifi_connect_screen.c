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
#include "esp_event.h"
#include "wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lcd_panel_rgb.h"

#define TAG "lvgl_wifi_connect"

extern esp_lcd_panel_handle_t panel_handle;
void wifi_event_callback(void *arg, esp_event_base_t event_base,
                         int32_t event_id, void *event_data);
extern esp_netif_t *sta_netif;
void wifi_connect_task(void *args)
{
    user_wifi_cfg *cfg = (user_wifi_cfg *)args;

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
        }
    };
    if (cfg)
    {
        strlcpy((char *)wifi_config.sta.ssid, (const char *)cfg->ssid, sizeof(wifi_config.sta.ssid));
        strlcpy((char *)wifi_config.sta.password, (const char *)cfg->password, sizeof(wifi_config.sta.password));
    }

    ESP_LOGI(TAG, "connecting... ssid: %s, password: %s", wifi_config.sta.ssid, wifi_config.sta.password);
    if (sta_netif == NULL)
    {
        sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);
    }
    esp_lcd_rgb_panel_set_pclk(panel_handle, 5 * 1000 * 1000);  // 连接wifi前临时降低刷新率防止屏幕偏移
    vTaskDelay(pdMS_TO_TICKS(50)); // 延时50ms 等待屏幕刷完一帧
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
    vTaskDelete(NULL);
}
#endif


void setup_scr_wifi_connect_screen(lv_ui *ui)
{
    //Write codes wifi_connect_screen
    ui->wifi_connect_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->wifi_connect_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->wifi_connect_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for wifi_connect_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_connect_screen_wifi_container
    ui->wifi_connect_screen_wifi_container = lv_obj_create(ui->wifi_connect_screen);
    lv_obj_set_pos(ui->wifi_connect_screen_wifi_container, 40, 40);
    lv_obj_set_size(ui->wifi_connect_screen_wifi_container, 400, 96);
    lv_obj_set_scrollbar_mode(ui->wifi_connect_screen_wifi_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for wifi_connect_screen_wifi_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_connect_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_screen_wifi_container, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen_wifi_container, 101, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_screen_wifi_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_screen_wifi_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_connect_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_connect_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_connect_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_connect_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_connect_screen_ssid
    ui->wifi_connect_screen_ssid = lv_label_create(ui->wifi_connect_screen_wifi_container);
    lv_obj_set_pos(ui->wifi_connect_screen_ssid, 7, 8);
    lv_obj_set_size(ui->wifi_connect_screen_ssid, 384, 26);
    lv_label_set_text(ui->wifi_connect_screen_ssid, "SSID");
    lv_label_set_long_mode(ui->wifi_connect_screen_ssid, LV_LABEL_LONG_SCROLL_CIRCULAR);

    //Write style for wifi_connect_screen_ssid, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_connect_screen_ssid, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_connect_screen_ssid, &lv_font_JetBrainsMono_Medium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_connect_screen_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_connect_screen_ssid, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_screen_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_connect_screen_password_input
    ui->wifi_connect_screen_password_input = lv_textarea_create(ui->wifi_connect_screen_wifi_container);
    lv_obj_set_pos(ui->wifi_connect_screen_password_input, 8, 48);
    lv_obj_set_size(ui->wifi_connect_screen_password_input, 384, 40);
    lv_textarea_set_text(ui->wifi_connect_screen_password_input, "");
    lv_textarea_set_placeholder_text(ui->wifi_connect_screen_password_input, "password");
    lv_textarea_set_password_bullet(ui->wifi_connect_screen_password_input, "*");
    lv_textarea_set_password_mode(ui->wifi_connect_screen_password_input, false);
    lv_textarea_set_one_line(ui->wifi_connect_screen_password_input, false);
    lv_textarea_set_accepted_chars(ui->wifi_connect_screen_password_input, "");
    lv_textarea_set_max_length(ui->wifi_connect_screen_password_input, 32);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->wifi_connect_screen_password_input, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for wifi_connect_screen_password_input, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->wifi_connect_screen_password_input, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_connect_screen_password_input, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_connect_screen_password_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_connect_screen_password_input, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_connect_screen_password_input, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen_password_input, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_screen_password_input, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_screen_password_input, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_connect_screen_password_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_screen_password_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_connect_screen_password_input, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_connect_screen_password_input, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_connect_screen_password_input, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_screen_password_input, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for wifi_connect_screen_password_input, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen_password_input, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_screen_password_input, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_screen_password_input, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_screen_password_input, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes wifi_connect_screen_cont_1
    ui->wifi_connect_screen_cont_1 = lv_obj_create(ui->wifi_connect_screen);
    lv_obj_set_pos(ui->wifi_connect_screen_cont_1, 40, 180);
    lv_obj_set_size(ui->wifi_connect_screen_cont_1, 400, 60);
    lv_obj_set_scrollbar_mode(ui->wifi_connect_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for wifi_connect_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_connect_screen_conn_btn
    ui->wifi_connect_screen_conn_btn = lv_button_create(ui->wifi_connect_screen_cont_1);
    lv_obj_set_pos(ui->wifi_connect_screen_conn_btn, 230, 10);
    lv_obj_set_size(ui->wifi_connect_screen_conn_btn, 120, 40);
    ui->wifi_connect_screen_conn_btn_label = lv_label_create(ui->wifi_connect_screen_conn_btn);
    lv_label_set_text(ui->wifi_connect_screen_conn_btn_label, "连接");
    lv_label_set_long_mode(ui->wifi_connect_screen_conn_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->wifi_connect_screen_conn_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->wifi_connect_screen_conn_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->wifi_connect_screen_conn_btn_label, LV_PCT(100));

    //Write style for wifi_connect_screen_conn_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen_conn_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_screen_conn_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_screen_conn_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_connect_screen_conn_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_screen_conn_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_screen_conn_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_connect_screen_conn_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_connect_screen_conn_btn, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_connect_screen_conn_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_connect_screen_conn_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_connect_screen_cancel_btn
    ui->wifi_connect_screen_cancel_btn = lv_button_create(ui->wifi_connect_screen_cont_1);
    lv_obj_set_pos(ui->wifi_connect_screen_cancel_btn, 50, 10);
    lv_obj_set_size(ui->wifi_connect_screen_cancel_btn, 120, 40);
    ui->wifi_connect_screen_cancel_btn_label = lv_label_create(ui->wifi_connect_screen_cancel_btn);
    lv_label_set_text(ui->wifi_connect_screen_cancel_btn_label, "取消");
    lv_label_set_long_mode(ui->wifi_connect_screen_cancel_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->wifi_connect_screen_cancel_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->wifi_connect_screen_cancel_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->wifi_connect_screen_cancel_btn_label, LV_PCT(100));

    //Write style for wifi_connect_screen_cancel_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_connect_screen_cancel_btn, 109, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_screen_cancel_btn, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_screen_cancel_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_connect_screen_cancel_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_screen_cancel_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_screen_cancel_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_connect_screen_cancel_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_connect_screen_cancel_btn, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_connect_screen_cancel_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_connect_screen_cancel_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of wifi_connect_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->wifi_connect_screen);

    //Init events for screen.
    events_init_wifi_connect_screen(ui);
}
