#include <stdio.h>
#include "ota.h"
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_app_format.h"
#include "esp_http_client.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "errno.h"
#include "cJSON.h"
#include "esp_crt_bundle.h"
#include "wifi.h"
#include "rgb_lcd.h"
#include "nvs_helper.h"

#define OTA_WRITE_BUFFSIZE 4096
static char* ota_write_data = NULL;
static const char *TAG = "ota";
static volatile double ota_download_progress = 0; // OTA下载进度
volatile uint8_t ota_status = OTA_STATUS_IDLE; // OTA状态
/*an ota data write buffer ready to write to the flash*/

static void http_cleanup(esp_http_client_handle_t client)
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

static void delete_ota_task()
{
    ota_download_progress = 0;
    if (ota_write_data != NULL)
    {
        free(ota_write_data);
        ota_write_data = NULL;
    }
    ota_status = OTA_STATUS_FAILED;
    rgb_lcd_set_pclk(10 * 1000 * 1000);
    vTaskDelete(NULL);
}

static void ota_task(void *pvParameter)
{
    ota_write_data = malloc(OTA_WRITE_BUFFSIZE + 1);
    rgb_lcd_set_pclk(5 * 1000 * 1000); // 降低PCLK频率，减少OTA过程中LCD的干扰
    esp_err_t err;
    /* update handle : set by esp_ota_begin(), must be freed via esp_ota_end() */
    esp_ota_handle_t update_handle = 0;
    const esp_partition_t *update_partition = NULL;

    ESP_LOGI(TAG, "Starting OTA example task");

    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();

    if (configured != running)
    {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08" PRIx32 ", but running from offset 0x%08" PRIx32,
                 configured->address, running->address);
        ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }
    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08" PRIx32 ")",
             running->type, running->subtype, running->address);

    esp_http_client_config_t config = {
        .url = ota_settings.download_url,
        .timeout_ms = 20000,
        .keep_alive_enable = true,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialise HTTP connection");
        delete_ota_task();
    }

    err = esp_http_client_open(client, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        delete_ota_task();
    }
    esp_http_client_fetch_headers(client);

    int64_t content_length = esp_http_client_get_content_length(client);

    update_partition = esp_ota_get_next_update_partition(NULL);
    assert(update_partition != NULL);
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%" PRIx32,
             update_partition->subtype, update_partition->address);

    int binary_file_length = 0;
    /*deal with all receive packet*/
    bool image_header_was_checked = false;
    while (1)
    {
        ota_status = OTA_STATUS_DOWNLOADING;
        int data_read = esp_http_client_read(client, ota_write_data, OTA_WRITE_BUFFSIZE);
        if (data_read < 0)
        {
            ESP_LOGE(TAG, "Error: SSL data read error");
            http_cleanup(client);
            delete_ota_task();
        }
        else if (data_read > 0)
        {
            if (image_header_was_checked == false)
            {
                esp_app_desc_t new_app_info;
                if (data_read > sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t))
                {
                    // check current version with downloading
                    memcpy(&new_app_info, &ota_write_data[sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t)], sizeof(esp_app_desc_t));
                    ESP_LOGI(TAG, "New firmware version: %s", new_app_info.version);

                    esp_app_desc_t running_app_info;
                    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK)
                    {
                        ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
                    }

                    const esp_partition_t *last_invalid_app = esp_ota_get_last_invalid_partition();
                    esp_app_desc_t invalid_app_info;
                    if (esp_ota_get_partition_description(last_invalid_app, &invalid_app_info) == ESP_OK)
                    {
                        ESP_LOGI(TAG, "Last invalid firmware version: %s", invalid_app_info.version);
                    }

                    // check current version with last invalid partition
                    if (last_invalid_app != NULL)
                    {
                        if (memcmp(invalid_app_info.version, new_app_info.version, sizeof(new_app_info.version)) == 0)
                        {
                            ESP_LOGW(TAG, "New version is the same as invalid version.");
                            ESP_LOGW(TAG, "Previously, there was an attempt to launch the firmware with %s version, but it failed.", invalid_app_info.version);
                            ESP_LOGW(TAG, "The firmware has been rolled back to the previous version.");
                            http_cleanup(client);
                            delete_ota_task();
                        }
                    }

                    image_header_was_checked = true;

                    err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
                    if (err != ESP_OK)
                    {
                        ESP_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
                        http_cleanup(client);
                        esp_ota_abort(update_handle);
                        delete_ota_task();
                    }
                    ESP_LOGI(TAG, "esp_ota_begin succeeded");
                }
                else
                {
                    ESP_LOGE(TAG, "received package is not fit len");
                    http_cleanup(client);
                    esp_ota_abort(update_handle);
                    delete_ota_task();
                }
            }
            // 获取下载进度
            ota_download_progress = (binary_file_length + data_read) * 100.0 / content_length;
            err = esp_ota_write(update_handle, (const void *)ota_write_data, data_read);
            if (err != ESP_OK)
            {
                http_cleanup(client);
                esp_ota_abort(update_handle);
                delete_ota_task();
            }
            binary_file_length += data_read;
            ESP_LOGD(TAG, "Written image length %d", binary_file_length);
        }
        else if (data_read == 0)
        {
            /*
             * As esp_http_client_read never returns negative error code, we rely on
             * `errno` to check for underlying transport connectivity closure if any
             */
            if (errno == ECONNRESET || errno == ENOTCONN)
            {
                ESP_LOGE(TAG, "Connection closed, errno = %d", errno);
                break;
            }
            if (esp_http_client_is_complete_data_received(client) == true)
            {
                ESP_LOGI(TAG, "Connection closed");
                break;
            }
        }
    }
    ESP_LOGI(TAG, "Total Write binary data length: %d", binary_file_length);
    if (esp_http_client_is_complete_data_received(client) != true)
    {
        ESP_LOGE(TAG, "Error in receiving complete file");
        http_cleanup(client);
        esp_ota_abort(update_handle);
        delete_ota_task();
    }

    err = esp_ota_end(update_handle);
    if (err != ESP_OK)
    {
        if (err == ESP_ERR_OTA_VALIDATE_FAILED)
        {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        }
        else
        {
            ESP_LOGE(TAG, "esp_ota_end failed (%s)!", esp_err_to_name(err));
        }
        http_cleanup(client);
        delete_ota_task();
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
        http_cleanup(client);
        delete_ota_task();
    }
    ESP_LOGI(TAG, "Prepare to restart system!");
    esp_restart();
    return;
}

#define MAX_HTTP_RECV_BUFFER 1024

static void get_ota_info_task(void *args)
{
    ota_status = OTA_STATUS_CHECKING_UPDATE;
    char *buffer = malloc(MAX_HTTP_RECV_BUFFER + 1);
    if (buffer == NULL)
    {
        ESP_LOGE(TAG, "Cannot malloc http receive buffer");
        return;
    }
    esp_http_client_config_t config = {
        .url = ota_settings.info_url,
        .disable_auto_redirect = true,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    ESP_LOGI(TAG, "HTTP Stream reader request =>");
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err;

    if (client == NULL)
    {
        ota_status = OTA_STATUS_FAILED;
        ESP_LOGE(TAG, "Failed to initialise HTTP connection");
        free(buffer);
        vTaskDelete(NULL);
    }

    if ((err = esp_http_client_open(client, 0)) != ESP_OK)
    {
        ota_status = OTA_STATUS_FAILED;
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        free(buffer);
        vTaskDelete(NULL);
    }
    int content_length = esp_http_client_fetch_headers(client);
    int total_read_len = 0, read_len;
    if (total_read_len < content_length && content_length <= MAX_HTTP_RECV_BUFFER)
    {
        read_len = esp_http_client_read(client, buffer, content_length);
        if (read_len <= 0)
        {
            ESP_LOGE(TAG, "Error read data");
        }
        buffer[read_len] = 0;
        ESP_LOGI(TAG, "read_len = %d", read_len);
    }
    ESP_LOGI(TAG, "HTTP Stream reader Status = %d, content_length = %" PRId64,
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
    ESP_LOGI(TAG, "Received data: %s", buffer);

    cJSON *root = cJSON_Parse(buffer);
    strcpy(ota_settings.download_url, cJSON_GetObjectItem(root, "download_url")->valuestring);
    ESP_LOGI(TAG, "download_url = %s", ota_settings.download_url);
    strcpy(ota_settings.newest_version, cJSON_GetObjectItem(root, "version")->valuestring);
    ESP_LOGI(TAG, "newest_version = %s", ota_settings.newest_version);
    cJSON_Delete(root);
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    free(buffer);
    // 检查是否有新版本
    char* current_version = malloc(32);
    ota_get_current_version(current_version);
    if (strcmp(current_version, ota_settings.newest_version) != 0)
    {
        ESP_LOGI(TAG, "Found new version %s, current version is %s", ota_settings.newest_version, current_version);
        ota_status = OTA_STATUS_FOUND_UPDATE;
    }
    else
    {
        ESP_LOGI(TAG, "Current version %s is the latest version", current_version);
        ota_status = OTA_STATUS_NO_UPDATE;
    }
    free(current_version);
    vTaskDelete(NULL);
}

void ota_start(void)
{
    xTaskCreate(&ota_task, "ota_task", 16384, NULL, 5, NULL);
}

void ota_check_for_update(void)
{
    xTaskCreate(&get_ota_info_task, "get_ota_info_task", 8192, NULL, 5, NULL);
}

void ota_get_current_version(char ota_version[])
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK)
    {
        ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
        strcpy(ota_version, running_app_info.version);
    }
}

double ota_get_download_progress(void)
{
    return ota_download_progress;
}