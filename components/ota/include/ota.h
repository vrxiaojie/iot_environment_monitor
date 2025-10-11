#pragma once 

enum{
    OTA_STATUS_IDLE = 0,
    OTA_STATUS_CHECKING_UPDATE,
    OTA_STATUS_NO_UPDATE,
    OTA_STATUS_FOUND_UPDATE,
    OTA_STATUS_DOWNLOADING,
    OTA_STATUS_FAILED
};

extern volatile uint8_t ota_status;

void ota_start(void);
void ota_check_for_update(void);
void ota_get_current_version(char ota_version[]);
