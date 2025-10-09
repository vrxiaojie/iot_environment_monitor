#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "wifi.h"
#include "rgb_lcd.h"

#define TAG "wifi_event_cb"

extern TaskHandle_t update_wifi_icon_task_handle;
extern TaskHandle_t wifi_status_change_task_handle;
static TaskHandle_t wifi_reconnect_task_handle = NULL;

void wifi_add_list_task(void *args);
void wifi_status_change_task(void *args);

static void wifi_reconnect_task(void *args)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        esp_lcd_rgb_panel_set_pclk(panel_handle, 5 * 1000 * 1000); // 连接wifi前临时降低刷新率防止屏幕偏移
        vTaskDelay(pdMS_TO_TICKS(50));                             // 延时50ms 等待屏幕刷完一帧
        esp_err_t err = esp_wifi_connect();
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to reconnect WiFi: %s", esp_err_to_name(err));
        }
    }
}

void wifi_event_callback(void *arg, esp_event_base_t event_base,
                         int32_t event_id, void *event_data)
{
    static uint8_t retry_cnt = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
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
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &xHigherPriorityTaskWoken);
        }
        else
        {
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &xHigherPriorityTaskWoken);
        }
        vTaskNotifyGiveFromISR(rgb_lcd_restart_panel_task_handle, &xHigherPriorityTaskWoken);
        vTaskNotifyGiveFromISR(update_wifi_icon_task_handle, &xHigherPriorityTaskWoken);
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
            vTaskNotifyGiveFromISR(rgb_lcd_restart_panel_task_handle, &xHigherPriorityTaskWoken);
        }
        else if (disconnected->reason == WIFI_REASON_ASSOC_LEAVE) // 手动断开连接(用于切换wifi)
        {
            ESP_LOGI(TAG, "WiFi stopped or connecting to new WiFi");
            retry_cnt = 0;
            vTaskNotifyGiveFromISR(rgb_lcd_restart_panel_task_handle, &xHigherPriorityTaskWoken);
        }
        else // 其他断开连接的情况均尝试重连
        {
            ESP_LOGI(TAG, "WiFi disconnected, reason: %d", disconnected->reason);
            // 重连尝试1次
            if (retry_cnt < 1)
            {
                retry_cnt++;
                vTaskNotifyGiveFromISR(wifi_reconnect_task_handle, &xHigherPriorityTaskWoken);
            }
            else
            {
                retry_cnt = 0;
                ESP_LOGI(TAG, "WiFi reconnect failed after 1 attempt");
                vTaskNotifyGiveFromISR(rgb_lcd_restart_panel_task_handle, &xHigherPriorityTaskWoken);
            }
        }

        if (wifi_status_change_task_handle == NULL)
        {
            xTaskCreatePinnedToCore(wifi_status_change_task, "wifi_status_change_task", 2 * 1024, NULL, 3, &wifi_status_change_task_handle, 1);
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &xHigherPriorityTaskWoken);
        }
        else
        {
            vTaskNotifyGiveFromISR(wifi_status_change_task_handle, &xHigherPriorityTaskWoken);
        }
        vTaskNotifyGiveFromISR(update_wifi_icon_task_handle, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void wifi_event_init()
{
    // 绑定wifi连接相关事件的回调函数
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &wifi_event_callback, NULL, NULL);
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_callback, NULL, NULL);
    // 绑定wifi扫描结束后的回调函数
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &wifi_event_callback, NULL, NULL);
    // 创建wifi重连任务
    if (wifi_reconnect_task_handle == NULL)
    {
        xTaskCreatePinnedToCore(wifi_reconnect_task, "wifi_reconnect_task", 4 * 1024, NULL, 3, &wifi_reconnect_task_handle, 1);
    }
}