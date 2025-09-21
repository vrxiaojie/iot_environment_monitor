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
#ifndef LV_USE_GUIDER_SIMULATOR // wifi设置页面专用自定义代码
#include "esp_event.h"
#include "wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lcd_panel_rgb.h"

#define TAG "lvgl_wifi_setup"
extern esp_lcd_panel_handle_t panel_handle;
extern _lock_t lvgl_api_lock; // LVGL API锁
// SSID选择回调函数
static void ssid_select_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        const char *ssid = lv_event_get_user_data(e);
        ESP_LOGI(TAG, "Selected WiFi SSID: %s", ssid);
        ui_load_scr_animation(&guider_ui, &guider_ui.wifi_connect_screen, guider_ui.wifi_connect_screen_del, &guider_ui.wifi_setting_screen_del, setup_scr_wifi_connect_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false, false);
        lv_label_set_text(guider_ui.wifi_connect_screen_ssid, ssid);
    }
}

// 清空WiFi列表
static void clear_wifi_list(void)
{
    if (guider_ui.wifi_setting_screen_wifi_scan_list)
    {
        _lock_acquire(&lvgl_api_lock);
        lv_obj_clean(guider_ui.wifi_setting_screen_wifi_scan_list);
        _lock_release(&lvgl_api_lock);
        ESP_LOGI(TAG, "WiFi list cleared");
    }
}

// 添加WiFi项到列表
static void add_wifi_item_to_list(const char *ssid, int8_t rssi, wifi_auth_mode_t auth_mode)
{
    // 创建WiFi项文本，包含SSID和信号强度
    char wifi_item_text[64];
    snprintf(wifi_item_text, sizeof(wifi_item_text), "%s (%ddBm)", ssid, rssi);
    _lock_acquire(&lvgl_api_lock);
    // 添加按钮到列表
    lv_obj_t *btn = lv_list_add_button(guider_ui.wifi_setting_screen_wifi_scan_list, LV_SYMBOL_WIFI, wifi_item_text);
    // 设置按钮文本样式
    lv_obj_set_style_text_font(btn, &lv_font_montserratMedium_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(btn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 为按钮添加点击事件
    lv_obj_add_event_cb(btn, ssid_select_cb, LV_EVENT_ALL, (void *)ssid);
    _lock_release(&lvgl_api_lock);
    ESP_LOGI(TAG, "Added WiFi item: %s", wifi_item_text);
}

static void wifi_add_list_task(void *args)
{
    uint16_t number = 16;

    // 清空之前的列表
    clear_wifi_list();

    /* 获取wifi扫描结果 */
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&number));
    number = number > 16 ? 16 : number; // 限制最大数量为16
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));

    // 获取上一次连接的WIFI名
    char last_connected_ssid[32] = {0};
    wifi_config_t wifi_config = {};
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &wifi_config));
    strlcpy(last_connected_ssid, (const char *)wifi_config.sta.ssid, sizeof(last_connected_ssid));
    bool last_ssid_found = false;

    // 将扫描到的WiFi添加到列表中
    for (int i = 0; i < number; i++)
    {
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);

        // 如果列表中有上一次连接的WIFI，则在扫描完成后尝试连接
        if (strcmp((const char *)ap_info[i].ssid, last_connected_ssid) == 0)
        {
            last_ssid_found = true;
        }

        add_wifi_item_to_list((char *)ap_info[i].ssid, ap_info[i].rssi, ap_info[i].authmode);

        // 主动让出CPU，避免长时间占用导致看门狗触发
        if ((i & 1) == 0) {
            vTaskDelay(1);
        } else {
            taskYIELD();
        }
    }
    _lock_acquire(&lvgl_api_lock);
    lv_obj_add_flag(guider_ui.wifi_setting_screen_wifi_scan_spinner, LV_OBJ_FLAG_HIDDEN); // 隐藏加载动画
    _lock_release(&lvgl_api_lock);
    // 如果找到了上次连接的WIFI，则尝试连接
    if (last_ssid_found)
    {
        wifi_connect_to_saved_ap();
    }
    ESP_LOGI(TAG, "WiFi scan results updated in UI");
    vTaskDelete(NULL);
}

// 连接成功后更新状态标签的任务
TaskHandle_t wifi_status_change_task_handle = NULL;
static void wifi_status_change_task(void *args)
{
    while (1)
    {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
        {
            if (wifi_sta_status == WIFI_CONNECTED && guider_ui.wifi_setting_screen_connect_status_label)
            {
                _lock_acquire(&lvgl_api_lock);
                lv_obj_set_style_text_color(guider_ui.wifi_setting_screen_connect_status_label, lv_color_hex(0x26c961), LV_PART_MAIN);
                _lock_release(&lvgl_api_lock);
            }
            else if (wifi_sta_status != WIFI_CONNECTED && guider_ui.wifi_setting_screen_connect_status_label)
            {
                _lock_acquire(&lvgl_api_lock);
                lv_obj_set_style_text_color(guider_ui.wifi_setting_screen_connect_status_label, lv_color_hex(0xE8202D), LV_PART_MAIN);
                _lock_release(&lvgl_api_lock);
            }
        }
    }
}

void wifi_event_callback(void *arg, esp_event_base_t event_base,
                         int32_t event_id, void *event_data)
{
    static uint8_t retry_cnt = 0;
    BaseType_t yiled = pdFALSE;
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_SCAN_DONE))
    {
        wifi_event_sta_scan_done_t *scan_done = (wifi_event_sta_scan_done_t *)event_data;
        if (scan_done->status == 0)
        {
            memset(ap_info, 0, sizeof(ap_info));
            xTaskCreatePinnedToCore(wifi_add_list_task, "wifi_add_list_task", 16 * 1024, NULL, 3, NULL, 1);
        }
        else
        {
            ESP_LOGE(TAG, "Scan failed");
        }
    }
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_CONNECTED))
    {
        retry_cnt = 0;
        wifi_sta_status = WIFI_CONNECTED;
        ESP_LOGI(TAG, "WiFi connected");
        if (wifi_status_change_task_handle == NULL)
        {
            xTaskCreatePinnedToCore(wifi_status_change_task, "wifi_status_change_task", 2 * 1024, NULL, 3, &wifi_status_change_task_handle, 1);
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &yiled);
        }
        else
        {
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &yiled);
        }
        esp_lcd_rgb_panel_set_pclk(panel_handle, 10 * 1000 * 1000);
        esp_lcd_rgb_panel_restart(panel_handle);
    }

    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_DISCONNECTED))
    {
        wifi_event_sta_disconnected_t *disconnected = (wifi_event_sta_disconnected_t *)event_data;
        wifi_sta_status = WIFI_DISCONNECTED;
        // wifi密码错误或认证过期时，不重连
        if (disconnected->reason == WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT || disconnected->reason == WIFI_REASON_AUTH_EXPIRE)
        {
            ESP_LOGW(TAG, "WiFi disconnected due to wrong password or auth expire. Reason: %d", disconnected->reason);
            retry_cnt = 0;
            esp_lcd_rgb_panel_set_pclk(panel_handle, 10 * 1000 * 1000);
            esp_lcd_rgb_panel_restart(panel_handle);
        }
        else if (disconnected->reason == WIFI_REASON_ASSOC_LEAVE) // 手动断开连接(用于切换wifi)
        {
            ESP_LOGI(TAG, "WiFi stopped or connecting to new WiFi");
            retry_cnt = 0;
            esp_lcd_rgb_panel_set_pclk(panel_handle, 10 * 1000 * 1000);
            esp_lcd_rgb_panel_restart(panel_handle);
        }
        else  // 其他断开连接的情况均尝试重连
        {
            ESP_LOGI(TAG, "WiFi disconnected, reason: %d", disconnected->reason);
            // 重连尝试1次
            if (retry_cnt < 1)
            {
                retry_cnt++;
                esp_lcd_rgb_panel_set_pclk(panel_handle, 5 * 1000 * 1000); // 连接wifi前临时降低刷新率防止屏幕偏移
                vTaskDelay(pdMS_TO_TICKS(20));                             // 延时20ms 等待屏幕刷完一帧
                ESP_ERROR_CHECK(esp_wifi_connect());
            }
            else
            {
                retry_cnt = 0;
                ESP_LOGI(TAG, "WiFi reconnect failed after 1 attempt");
                esp_lcd_rgb_panel_set_pclk(panel_handle, 10 * 1000 * 1000);
                esp_lcd_rgb_panel_restart(panel_handle);
            }
        }

        if (wifi_status_change_task_handle == NULL)
        {
            xTaskCreatePinnedToCore(wifi_status_change_task, "wifi_status_change_task", 2 * 1024, NULL, 3, &wifi_status_change_task_handle, 1);
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &yiled);
        }
        else
        {
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &yiled);
        }
    }
}
#endif


void setup_scr_wifi_setting_screen(lv_ui *ui)
{
    //Write codes wifi_setting_screen
    ui->wifi_setting_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->wifi_setting_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->wifi_setting_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for wifi_setting_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_setting_screen, lv_color_hex(0x1B1E2A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_setting_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_setting_screen_wifi_container
    ui->wifi_setting_screen_wifi_container = lv_obj_create(ui->wifi_setting_screen);
    lv_obj_set_pos(ui->wifi_setting_screen_wifi_container, 40, 40);
    lv_obj_set_size(ui->wifi_setting_screen_wifi_container, 400, 40);
    lv_obj_set_scrollbar_mode(ui->wifi_setting_screen_wifi_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for wifi_setting_screen_wifi_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_setting_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_setting_screen_wifi_container, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_wifi_container, 101, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_setting_screen_wifi_container, lv_color_hex(0x5b5b5b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_setting_screen_wifi_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_setting_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_setting_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_setting_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_setting_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_setting_screen_wifi_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_setting_screen_label_wifi
    ui->wifi_setting_screen_label_wifi = lv_label_create(ui->wifi_setting_screen_wifi_container);
    lv_obj_set_pos(ui->wifi_setting_screen_label_wifi, 6, 6);
    lv_obj_set_size(ui->wifi_setting_screen_label_wifi, 87, 28);
    lv_label_set_text(ui->wifi_setting_screen_label_wifi, "WIFI");
    lv_label_set_long_mode(ui->wifi_setting_screen_label_wifi, LV_LABEL_LONG_WRAP);

    //Write style for wifi_setting_screen_label_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_setting_screen_label_wifi, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_setting_screen_label_wifi, &lv_font_JetBrainsMono_Medium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_setting_screen_label_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_setting_screen_label_wifi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_setting_screen_label_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_setting_screen_wifi_switch
    ui->wifi_setting_screen_wifi_switch = lv_switch_create(ui->wifi_setting_screen_wifi_container);
    lv_obj_set_pos(ui->wifi_setting_screen_wifi_switch, 329, 9);
    lv_obj_set_size(ui->wifi_setting_screen_wifi_switch, 40, 20);

    //Write style for wifi_setting_screen_wifi_switch, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_wifi_switch, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_setting_screen_wifi_switch, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_setting_screen_wifi_switch, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_setting_screen_wifi_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_setting_screen_wifi_switch, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_setting_screen_wifi_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for wifi_setting_screen_wifi_switch, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_wifi_switch, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->wifi_setting_screen_wifi_switch, lv_color_hex(0x1a7f14), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->wifi_setting_screen_wifi_switch, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->wifi_setting_screen_wifi_switch, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for wifi_setting_screen_wifi_switch, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_wifi_switch, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_setting_screen_wifi_switch, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_setting_screen_wifi_switch, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_setting_screen_wifi_switch, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_setting_screen_wifi_switch, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes wifi_setting_screen_wifi_scan_spinner
    ui->wifi_setting_screen_wifi_scan_spinner = lv_spinner_create(ui->wifi_setting_screen_wifi_container);
    lv_obj_set_pos(ui->wifi_setting_screen_wifi_scan_spinner, 90, 12);
    lv_obj_set_size(ui->wifi_setting_screen_wifi_scan_spinner, 16, 16);
    lv_obj_add_flag(ui->wifi_setting_screen_wifi_scan_spinner, LV_OBJ_FLAG_HIDDEN);
    lv_spinner_set_anim_params(ui->wifi_setting_screen_wifi_scan_spinner, 2000, 200);

    //Write style for wifi_setting_screen_wifi_scan_spinner, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->wifi_setting_screen_wifi_scan_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_setting_screen_wifi_scan_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_setting_screen_wifi_scan_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_setting_screen_wifi_scan_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_wifi_scan_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->wifi_setting_screen_wifi_scan_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_setting_screen_wifi_scan_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for wifi_setting_screen_wifi_scan_spinner, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->wifi_setting_screen_wifi_scan_spinner, 4, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->wifi_setting_screen_wifi_scan_spinner, 123, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->wifi_setting_screen_wifi_scan_spinner, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->wifi_setting_screen_wifi_scan_spinner, true, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes wifi_setting_screen_network_info_btn
    ui->wifi_setting_screen_network_info_btn = lv_button_create(ui->wifi_setting_screen_wifi_container);
    lv_obj_set_pos(ui->wifi_setting_screen_network_info_btn, 210, 5);
    lv_obj_set_size(ui->wifi_setting_screen_network_info_btn, 80, 30);
    ui->wifi_setting_screen_network_info_btn_label = lv_label_create(ui->wifi_setting_screen_network_info_btn);
    lv_label_set_text(ui->wifi_setting_screen_network_info_btn_label, "网络信息");
    lv_label_set_long_mode(ui->wifi_setting_screen_network_info_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->wifi_setting_screen_network_info_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->wifi_setting_screen_network_info_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->wifi_setting_screen_network_info_btn_label, LV_PCT(100));

    //Write style for wifi_setting_screen_network_info_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_network_info_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_setting_screen_network_info_btn, lv_color_hex(0x0f4187), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_setting_screen_network_info_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_setting_screen_network_info_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_setting_screen_network_info_btn, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_setting_screen_network_info_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_setting_screen_network_info_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_setting_screen_network_info_btn, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_setting_screen_network_info_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_setting_screen_network_info_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_setting_screen_connect_status_label
    ui->wifi_setting_screen_connect_status_label = lv_label_create(ui->wifi_setting_screen_wifi_container);
    lv_obj_set_pos(ui->wifi_setting_screen_connect_status_label, 109, 12);
    lv_obj_set_size(ui->wifi_setting_screen_connect_status_label, 89, 22);
    lv_label_set_text(ui->wifi_setting_screen_connect_status_label, "" LV_SYMBOL_WIFI " ");
    lv_label_set_long_mode(ui->wifi_setting_screen_connect_status_label, LV_LABEL_LONG_WRAP);

    //Write style for wifi_setting_screen_connect_status_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_setting_screen_connect_status_label, lv_color_hex(0xE8202D), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_setting_screen_connect_status_label, &lv_font_xiaobiaosong_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_setting_screen_connect_status_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_setting_screen_connect_status_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_setting_screen_connect_status_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_setting_screen_wifi_scan_list
    ui->wifi_setting_screen_wifi_scan_list = lv_list_create(ui->wifi_setting_screen);
    lv_obj_set_pos(ui->wifi_setting_screen_wifi_scan_list, 40, 96);
    lv_obj_set_size(ui->wifi_setting_screen_wifi_scan_list, 400, 160);
    lv_obj_set_scrollbar_mode(ui->wifi_setting_screen_wifi_scan_list, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_setting_screen_wifi_scan_list_main_main_default
    static lv_style_t style_wifi_setting_screen_wifi_scan_list_main_main_default;
    ui_init_style(&style_wifi_setting_screen_wifi_scan_list_main_main_default);

    lv_style_set_pad_top(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 5);
    lv_style_set_pad_left(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 5);
    lv_style_set_pad_right(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 5);
    lv_style_set_pad_bottom(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 5);
    lv_style_set_bg_opa(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 100);
    lv_style_set_bg_color(&style_wifi_setting_screen_wifi_scan_list_main_main_default, lv_color_hex(0x5b5b5b));
    lv_style_set_bg_grad_dir(&style_wifi_setting_screen_wifi_scan_list_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 0);
    lv_style_set_radius(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 16);
    lv_style_set_shadow_width(&style_wifi_setting_screen_wifi_scan_list_main_main_default, 0);
    lv_obj_add_style(ui->wifi_setting_screen_wifi_scan_list, &style_wifi_setting_screen_wifi_scan_list_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default
    static lv_style_t style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default;
    ui_init_style(&style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default);

    lv_style_set_radius(&style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->wifi_setting_screen_wifi_scan_list, &style_wifi_setting_screen_wifi_scan_list_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default
    static lv_style_t style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default;
    ui_init_style(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default);

    lv_style_set_pad_top(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, &lv_font_montserratMedium_16);
    lv_style_set_text_opa(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 255);
    lv_style_set_radius(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, 118);
    lv_style_set_bg_color(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_wifi_setting_screen_wifi_scan_list_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default
    static lv_style_t style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default;
    ui_init_style(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default);

    lv_style_set_pad_top(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, lv_color_hex(0x1483f3));
    lv_style_set_text_font(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, &lv_font_montserratMedium_16);
    lv_style_set_text_opa(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 255);
    lv_style_set_radius(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_wifi_setting_screen_wifi_scan_list_extra_texts_main_default, 0);

    //The custom code of wifi_setting_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->wifi_setting_screen);

    //Init events for screen.
    events_init_wifi_setting_screen(ui);
}
