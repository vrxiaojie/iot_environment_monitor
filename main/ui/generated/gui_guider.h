/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *main_screen;
	bool main_screen_del;
	lv_obj_t *main_screen_temp_container;
	lv_obj_t *main_screen_temp_value;
	lv_obj_t *main_screen_temp_label;
	lv_obj_t *main_screen_temp_bar;
	lv_obj_t *main_screen_co2_container;
	lv_obj_t *main_screen_co2_value;
	lv_obj_t *main_screen_co2_label;
	lv_obj_t *main_screen_co2_bar;
	lv_obj_t *main_screen_humid_container;
	lv_obj_t *main_screen_humid_value;
	lv_obj_t *main_screen_humid_label;
	lv_obj_t *main_screen_humid_bar;
	lv_obj_t *main_screen_voc_container;
	lv_obj_t *main_screen_voc_value;
	lv_obj_t *main_screen_voc_label;
	lv_obj_t *main_screen_voc_bar;
	lv_obj_t *main_screen_time_container;
	lv_obj_t *main_screen_time_hour;
	lv_obj_t *main_screen_time_minute;
	lv_obj_t *main_screen_time_month_day;
	lv_obj_t *setting_screen;
	bool setting_screen_del;
	lv_obj_t *setting_screen_wifi_icon_container;
	lv_obj_t *setting_screen_wifi_icon;
	lv_obj_t *setting_screen_backlight_container;
	lv_obj_t *setting_screen_backlight_icon;
	lv_obj_t *setting_screen_backlight_slider;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_main_screen(lv_ui *ui);
void setup_scr_setting_screen(lv_ui *ui);
LV_IMAGE_DECLARE(_WIFI_RGB565A8_48x48);
LV_IMAGE_DECLARE(_backlight_RGB565A8_32x32);

LV_FONT_DECLARE(lv_font_JetBrainsMono_Medium_36)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_xiaobiaosong_16)
LV_FONT_DECLARE(lv_font_JetBrainsMono_Medium_40)
LV_FONT_DECLARE(lv_font_JetBrainsMono_Medium_24)


#ifdef __cplusplus
}
#endif
#endif
