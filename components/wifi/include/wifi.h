#pragma once
#include "esp_wifi.h"
#include "esp_netif.h"

typedef struct
{
    uint8_t ssid[32];
    uint8_t password[64];
} user_wifi_cfg;

typedef struct 
{
    char ip[16];
    char netmask[16];
    char gw[16];
} wifi_ip_info_t;


enum WIFISTATUS{
    WIFI_DISCONNECTED,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_FAILED,
    WIFI_SCANNING,
};

extern wifi_ap_record_t ap_info[16];
extern volatile uint8_t wifi_sta_status;

// WiFi functions
void wifi_init(void);
void wifi_start(void);
void wifi_stop(void);
void wifi_scan(void);
void wifi_connect(user_wifi_cfg *cfg);
void wifi_disconnect(void);
void wifi_get_ip_info_str(wifi_ip_info_t* wifi_ip_info);
bool wifi_connect_to_saved_ap(void);
bool is_wifi_connected(void);
void wifi_event_init(void);
