#pragma once

#include "sdkconfig.h"
#include <stdint.h>

extern volatile uint8_t backlight_duty;

void lcd_backlight_init(void);
void lcd_backlight_set_duty(uint8_t duty);
void lcd_backlight_get_duty(uint8_t *duty);