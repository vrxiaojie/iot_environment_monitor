#include "esp_sntp.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_netif_sntp.h"
#include "ntp.h"
#include "time.h"
#include "wifi.h"

void ntp_init()
{
    // 设置时区为中国标准时间
    setenv("TZ", "CST-8", 1);
    tzset();
    // 初始化SNTP
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(2,
                                                                      ESP_SNTP_SERVER_LIST("ntp.ntsc.ac.cn", "ntp1.aliyun.com"));
    esp_netif_sntp_init(&config);
}

void ntp_sync_task(void *arg)
{
    ntp_init();
    while (1)
    {
        if (wifi_sta_status == WIFI_CONNECTED)
        {
            if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK)
            {
                ESP_LOGW("NTP", "Failed to update system time within 10s timeout");
            }
            else
            {
                ESP_LOGI("NTP", "System time updated");
                vTaskDelay(pdMS_TO_TICKS(30 * 60 * 1000)); // 30mins同步一次
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
