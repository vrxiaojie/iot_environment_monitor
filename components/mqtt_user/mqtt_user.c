#include <stdio.h>
#include "mqtt_user.h"
#include "stcc4.h"
#include "sgp4x.h"

#define TAG "MQTT"

esp_mqtt_client_handle_t client = NULL;
TaskHandle_t mqtt_publish_task_handle = NULL;
TaskHandle_t mqtt_stop_task_handle = NULL;
extern TaskHandle_t update_mqtt_screen_task_handle;
extern STCC4_t stcc4;
extern int32_t voc_index;

volatile uint8_t mqtt_status = 0; // 0:未连接 1:已连接 2: 连接失败

static void mqtt_stop_task(void *args)
{
    if (mqtt_publish_task_handle)
    {
        vTaskDelete(mqtt_publish_task_handle);
        mqtt_publish_task_handle = NULL;
    }
    if (client)
    {
        esp_mqtt_client_disconnect(client);
        if (mqtt_status == 1) // 如果是已连接状态，断开连接后变为未连接状态
            mqtt_status = 0;
        vTaskDelay(100);
        esp_mqtt_client_stop(client);
        vTaskDelay(50);
        esp_mqtt_client_destroy(client);
    }
    if (update_mqtt_screen_task_handle)
        xTaskNotifyGive(update_mqtt_screen_task_handle);
    mqtt_stop_task_handle = NULL;
    vTaskDelete(NULL);
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        mqtt_status = 1;
        if (update_mqtt_screen_task_handle != NULL)
            vTaskNotifyGiveFromISR(update_mqtt_screen_task_handle, &xHigherPriorityTaskWoken);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        if (mqtt_status == 2)
            break; // 连接失败后会连着调用MQTT_EVENT_ERROR和MQTT_EVENT_DISCONNECTED，此时就不再修改状态
        mqtt_status = 0;
        if (update_mqtt_screen_task_handle != NULL)
            vTaskNotifyGiveFromISR(update_mqtt_screen_task_handle, &xHigherPriorityTaskWoken);
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("CONFIG_MQTT_PUBLISH_TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        mqtt_status = 2;
        mqtt_stop();
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_publish_data_task(void *args)
{
    char json_data[128];
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    while (1)
    {
        if (client && mqtt_get_status() == 1)
        {
            // 使用字符串处理，将传感器数据格式化为JSON字符串
            snprintf(json_data, sizeof(json_data),
                     "{\"temp\":%.2f,\"humid\":%.2f,\"voc\":%ld,\"co2\":%d}",
                     stcc4.temperature,
                     stcc4.relativeHumidity,
                     voc_index,
                     stcc4.co2Concentration);
            esp_mqtt_client_publish(client, CONFIG_MQTT_PUBLISH_TOPIC, json_data, 0, 1, 0);
        }
        if (mqtt_user_config.upload_interval >= 5)
            vTaskDelay(pdMS_TO_TICKS(mqtt_user_config.upload_interval * 1000));
        else
            vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void mqtt_start()
{
    // get_mqtt_user_config
    mqtt_read_settings();
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = mqtt_user_config.uri,
        .broker.address.port = mqtt_user_config.port,
        .credentials.username = mqtt_user_config.username,
        .credentials.authentication.password = mqtt_user_config.password,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_publish_task_handle == NULL)
    {
        xTaskCreatePinnedToCoreWithCaps(mqtt_publish_data_task, "mqtt_publish_data_task", 8 * 1024, NULL, 3, &mqtt_publish_task_handle, 1, MALLOC_CAP_SPIRAM);
    }
    esp_mqtt_client_start(client);
}

void mqtt_stop()
{
    if (mqtt_stop_task_handle == NULL)
        xTaskCreateWithCaps(mqtt_stop_task, "mqtt_stop_task", 4 * 1024, NULL, 5, &mqtt_stop_task_handle, MALLOC_CAP_SPIRAM);
}

uint8_t mqtt_get_status()
{
    return mqtt_status;
}
