#pragma once

#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_helper.h"
#include "wifi.h"

extern esp_mqtt_client_handle_t client;
extern volatile uint8_t mqtt_status;

void mqtt_start();
void mqtt_stop();
uint8_t mqtt_get_status();