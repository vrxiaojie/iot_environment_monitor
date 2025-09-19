#include <stdio.h>
#include "wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"

#define TAG "wifi"

TaskHandle_t wifi_scan_task_handle = NULL;

wifi_ap_record_t ap_info[16];

esp_netif_t *sta_netif = NULL;

uint8_t wifi_sta_status = WIFI_DISCONNECTED;

static void print_auth_mode(int authmode)
{
    switch (authmode)
    {
    case WIFI_AUTH_OPEN:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OPEN");
        break;
    case WIFI_AUTH_OWE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OWE");
        break;
    case WIFI_AUTH_WEP:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_PSK");
        break;
    case WIFI_AUTH_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
        break;
    case WIFI_AUTH_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA2_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA3_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA2_WPA3_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA3_ENT_192:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_ENT_192");
        break;
    default:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_UNKNOWN");
        break;
    }
}

static void print_cipher_type(int pairwise_cipher, int group_cipher)
{
    switch (pairwise_cipher)
    {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    case WIFI_CIPHER_TYPE_AES_CMAC128:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_AES_CMAC128");
        break;
    case WIFI_CIPHER_TYPE_SMS4:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_SMS4");
        break;
    case WIFI_CIPHER_TYPE_GCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_GCMP");
        break;
    case WIFI_CIPHER_TYPE_GCMP256:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_GCMP256");
        break;
    default:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }

    switch (group_cipher)
    {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    case WIFI_CIPHER_TYPE_SMS4:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_SMS4");
        break;
    case WIFI_CIPHER_TYPE_GCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP");
        break;
    case WIFI_CIPHER_TYPE_GCMP256:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP256");
        break;
    default:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }
}

void __attribute__((weak)) wifi_event_callback(void *arg, esp_event_base_t event_base,
                                               int32_t event_id, void *event_data)
{
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_SCAN_DONE))
    {
        uint16_t ap_count = 0;
        wifi_event_sta_scan_done_t *scan_done = (wifi_event_sta_scan_done_t *)event_data;

        ESP_LOGI(TAG, "wifi scan done, status:%lu, number:%u", scan_done->status, scan_done->number);
        // 0: success, 1: failure
        if (scan_done->status == 0)
        {
            uint16_t number = 16; // 最多获取16个WIFI

            memset(ap_info, 0, sizeof(ap_info));
            /* 获取wifi扫描结果 */
            ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
            ESP_LOGI(TAG, "Total APs scanned = %u, actual AP number ap_info holds = %u", ap_count, number);
            for (int i = 0; i < number; i++)
            {
                ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
                ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
                print_auth_mode(ap_info[i].authmode);
                if (ap_info[i].authmode != WIFI_AUTH_WEP)
                {
                    print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
                }
                ESP_LOGI(TAG, "Channel \t\t%d", ap_info[i].primary);
            }
        }
        else
        {
            ESP_LOGE(TAG, "Scan failed");
        }
    }
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_CONNECTED))
    {
        ESP_LOGI(TAG, "WiFi connected");
    }
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_DISCONNECTED))
    {
        ESP_LOGI(TAG, "WiFi disconnected");
    }
}

void __attribute__((weak)) wifi_connect_task(void *args)
{
    user_wifi_cfg *cfg = (user_wifi_cfg *)args;

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
        }};
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

    // wifi连接相关事件绑定
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &wifi_event_callback, NULL, NULL);
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_callback, NULL, NULL);
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
    vTaskDelete(NULL);
}

static void wifi_scan_task(void *args)
{
    if (sta_netif == NULL)
    {
        sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);
    }

    while (1)
    {
        // 使用任务通知接收
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == 1)
        {
            vTaskDelay(pdMS_TO_TICKS(200)); // 延时1秒，等待wifi启动完成
            // 开始扫描
            ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, false)); // false: 异步扫描，扫描完成后会触发 WIFI_EVENT_SCAN_DONE 事件
            ESP_LOGI(TAG, "Started wifi scan");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void wifi_start()
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void wifi_stop()
{
    ESP_ERROR_CHECK(esp_wifi_stop());
}

void wifi_scan()
{
    if (wifi_scan_task_handle != NULL)
    {
        // 通知任务开始扫描
        xTaskNotifyGive(wifi_scan_task_handle);
        ESP_LOGI(TAG, "Notified wifi_scan_task to start scanning");
    }
    else
    {
        ESP_LOGE(TAG, "wifi_scan_task_handle is NULL");
    }
}

void wifi_connect(user_wifi_cfg *cfg)
{
    BaseType_t xReturned = xTaskCreate(wifi_connect_task, "wifi_connect_task", 8 * 1024, (void *)cfg, 6, NULL);
    if (xReturned != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create wifi_connect_task");
    }
}

void wifi_init()
{
    // 初始化wifi前需要初始化NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_netif_init()); // 初始化TCP/IP栈
    // 创建wifi扫描任务
    BaseType_t xReturned = xTaskCreate(wifi_scan_task, "wifi_scan_task", 8 * 1024, NULL, 15, &wifi_scan_task_handle);
    if (xReturned != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create wifi_scan_task");
    }
    // wifi连接相关事件绑定
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &wifi_event_callback, NULL, NULL);
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_callback, NULL, NULL);
    // wifi扫描结束后的回调函数绑定
    esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &wifi_event_callback, NULL, NULL);
}

void wifi_get_ip_info_str(wifi_ip_info_t* wifi_ip_info)
{
    if (sta_netif != NULL)
    {
        esp_netif_ip_info_t *netif_ip_info = malloc(sizeof(esp_netif_ip_info_t));
        esp_netif_get_ip_info(sta_netif, netif_ip_info);
        sprintf(wifi_ip_info->ip, IPSTR, IP2STR(&netif_ip_info->ip));
        sprintf(wifi_ip_info->netmask, IPSTR, IP2STR(&netif_ip_info->netmask));
        sprintf(wifi_ip_info->gw, IPSTR, IP2STR(&netif_ip_info->gw));
        free(netif_ip_info);
    }
}
