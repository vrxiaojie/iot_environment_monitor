#pragma once

#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_helper.h"

extern esp_mqtt_client_handle_t client;
extern bool mqtt_status;

void mqtt_start();
void mqtt_stop();
bool is_mqtt_connected();