#pragma once

#include "driver/ledc.h"
#include "sdkconfig.h"

void lcd_backlight_init(void);
void lcd_backlight_set_duty(uint8_t duty);
void lcd_backlight_get_duty(uint8_t *duty);