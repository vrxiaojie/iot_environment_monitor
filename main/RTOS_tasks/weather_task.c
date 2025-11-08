#include "weather.h"
#include "RTOS_tasks.h"
#include "wifi.h"
#include "lvgl.h"
#include "gui_guider.h"
#include <sys/lock.h>



TaskHandle_t weather_task_handle = NULL;
extern _lock_t lvgl_api_lock;

void weather_task(void *arg)
{
    char temp_str[8];
    char humid_str[8];
    char wind_speed_str[8];
    char feel_temp_str[8];
    char pressure_str[8];
    char visi_str[8];
    char weather_str[16];
    char precip_str[8];
    weather_config_t config = {
        .api_key = API_KEY,
        .api_host = HOST,
        .city = "beijing"};
    while (1)
    {
        if (is_wifi_connected())
        {
            weather_info = weather_get(&config);
            if (weather_info)
            {
                weather_print_info(weather_info);
                snprintf(temp_str, sizeof(temp_str), "%d", (int)weather_info->temperature);
                snprintf(humid_str, sizeof(humid_str), "%d", (int)weather_info->humidity);
                snprintf(wind_speed_str, sizeof(wind_speed_str), "%d", (int)weather_info->wind_speed);
                snprintf(feel_temp_str, sizeof(feel_temp_str), "%d", (int)weather_info->feels_like);
                snprintf(pressure_str, sizeof(pressure_str), "%d", (int)weather_info->pressure);
                snprintf(visi_str, sizeof(visi_str), "%d", (int)weather_info->visibility);
                snprintf(weather_str, sizeof(weather_str), "%s", weather_info->weather);
                snprintf(precip_str, sizeof(precip_str), "%.1fmm", weather_info->precip);
                _lock_acquire(&lvgl_api_lock);
                lv_label_set_text(guider_ui.weather_screen_temp_value, temp_str);
                lv_label_set_text(guider_ui.weather_screen_humid_value, humid_str);
                lv_label_set_text(guider_ui.weather_screen_wind_speed_value, wind_speed_str);
                lv_label_set_text(guider_ui.weather_screen_feel_temp_value, feel_temp_str);
                lv_label_set_text(guider_ui.weather_screen_pressure_value, pressure_str);
                lv_label_set_text(guider_ui.weather_screen_visi_value, visi_str);
                lv_label_set_text(guider_ui.weather_screen_weather_label, weather_str);
                lv_label_set_text(guider_ui.weather_screen_precip_value, precip_str);
                _lock_release(&lvgl_api_lock);
            }
        }
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000 * 60 * 30)); // 每30分钟获取一次天气数据
    }
}
