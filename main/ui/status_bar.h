#pragma once
#include <stdbool.h>

void status_bar_create(void);
void status_bar_set_time(const char *time_str);
void status_bar_set_wifi_state(bool connected);
void status_bar_set_battery_level(float level);
void status_bar_set_charge_state(bool charging);
void status_bar_show();
void status_bar_hide();