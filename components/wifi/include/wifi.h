#pragma once
#include "esp_wifi.h"
#include "esp_netif.h"

typedef struct
{
    uint8_t ssid[32];
    uint8_t password[64];
} user_wifi_cfg;

extern wifi_ap_record_t ap_info[16];

// WiFi functions
void wifi_init(void);
void wifi_start(void);
void wifi_stop(void);
void wifi_scan(void);
void wifi_connect(user_wifi_cfg *cfg);
