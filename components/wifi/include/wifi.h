#pragma once
#include "esp_wifi.h"
#include "esp_netif.h"

// WiFi functions
void wifi_init(void);
void wifi_start(void);
void wifi_stop(void);
void wifi_scan(void);

extern wifi_ap_record_t ap_info[16];