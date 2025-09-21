#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_lcd_panel_rgb.h"
#include "wifi.h"

#define TAG "wifi_event_cb"

extern TaskHandle_t update_wifi_icon_task_handle;
extern TaskHandle_t wifi_status_change_task_handle;
extern esp_lcd_panel_handle_t panel_handle;

void wifi_add_list_task(void *args);
void wifi_status_change_task(void *args);

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
        xTaskNotifyGive(update_wifi_icon_task_handle);
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
        xTaskNotifyGive(update_wifi_icon_task_handle);
    }
}