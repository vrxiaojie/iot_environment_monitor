#include "data_chart.h"

extern STCC4_t stcc4;
extern int32_t voc_index;
chart_data_t chart_data = {0};
extern _lock_t lvgl_api_lock;
time_frame_t current_time_frame = TIME_FRAME_1MIN;
chart_type_t current_chart_type = CHART_TYPE_TEMPERATURE;

TaskHandle_t update_chart_task_handle = NULL;
TaskHandle_t get_data_task_handle = NULL;

static char str0[12], str1[12], str2[12];
static const char *y_scale[] = {str0, str1, str2, NULL}; // Y轴刻度

lv_obj_t *chart;
lv_chart_series_t *ser1;
lv_obj_t *scale_left;
lv_obj_t *scale_bottom;

static int32_t get_max_value(int32_t *array, uint8_t size)
{
    int32_t max = array[0];
    for (uint8_t i = 0; i < size; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    return max;
}

static int32_t get_min_value(int32_t *array, uint8_t size)
{
    int32_t min = array[0];
    for (uint8_t i = 0; i < size; i++)
    {
        if (array[i] < min)
        {
            min = array[i];
        }
    }
    return min;
}

void update_chart_x_scale_text()
{
    switch (current_time_frame)
    {
    case TIME_FRAME_1MIN:
        static const char *time_arr_1min[] = {"55s", "45s", "35s", "25s", "15s", "5s", NULL};
        lv_scale_set_total_tick_count(scale_bottom, 12);
        lv_scale_set_major_tick_every(scale_bottom, 2);
        lv_scale_set_text_src(scale_bottom, time_arr_1min);
        break;
    case TIME_FRAME_1HOUR:
        static const char *time_arr_1hour[] = {"55m", "45m", "35m", "25m", "15m", "5m", NULL};
        lv_scale_set_total_tick_count(scale_bottom, 12);
        lv_scale_set_major_tick_every(scale_bottom, 2);
        lv_scale_set_text_src(scale_bottom, time_arr_1hour);
        break;
    case TIME_FRAME_1DAY:
        static const char *time_arr_1day[] = {"23h", "19h", "15h", "11h", "7h", "3h", NULL};
        lv_scale_set_total_tick_count(scale_bottom, 24);
        lv_scale_set_major_tick_every(scale_bottom, 4);
        lv_scale_set_text_src(scale_bottom, time_arr_1day);
        break;
    }
}

void set_x_tick_count()
{
    switch (current_time_frame)
    {
    case TIME_FRAME_1MIN:
        lv_chart_set_point_count(chart, 12);
        lv_scale_set_total_tick_count(scale_bottom, 12);
        break;
    case TIME_FRAME_1HOUR:
        lv_chart_set_point_count(chart, 12);
        lv_scale_set_total_tick_count(scale_bottom, 12);
        break;
    case TIME_FRAME_1DAY:
        lv_chart_set_point_count(chart, 24);
        lv_scale_set_total_tick_count(scale_bottom, 24);
        break;
    }
}

void update_chart_task(void *arg)
{
    int32_t max = 0, min = 0, mid = 0;
    while (1)
    {
        _lock_acquire(&lvgl_api_lock);
        // 根据 current_chart_type 和 current_time_frame 选择对应的数据数组
        switch (current_chart_type)
        {
        case CHART_TYPE_CO2:
            switch (current_time_frame)
            {
            case TIME_FRAME_1MIN:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.co2.oneMinute);
                min = get_min_value(chart_data.co2.oneMinute, 12);
                max = get_max_value(chart_data.co2.oneMinute, 12);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.co2.oneHour);
                min = get_min_value(chart_data.co2.oneHour, 12);
                max = get_max_value(chart_data.co2.oneHour, 12);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.co2.oneDay);
                min = get_min_value(chart_data.co2.oneDay, 24);
                max = get_max_value(chart_data.co2.oneDay, 24);
                break;
            }
            min = min < 300 ? 0 : min - 300;
            max = max > 5000 ? 5000 : max + 300;
            break;
        case CHART_TYPE_TEMPERATURE:
            switch (current_time_frame)
            {
            case TIME_FRAME_1MIN:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.temperature.oneMinute);
                min = get_min_value(chart_data.temperature.oneMinute, 12);
                max = get_max_value(chart_data.temperature.oneMinute, 12);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.temperature.oneHour);
                min = get_min_value(chart_data.temperature.oneHour, 12);
                max = get_max_value(chart_data.temperature.oneHour, 12);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.temperature.oneDay);
                min = get_min_value(chart_data.temperature.oneDay, 24);
                max = get_max_value(chart_data.temperature.oneDay, 24);
                break;
            }
            min -= 10;
            max += 10;
            break;
        case CHART_TYPE_HUMIDITY:
            switch (current_time_frame)
            {
            case TIME_FRAME_1MIN:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.humidity.oneMinute);
                min = get_min_value(chart_data.humidity.oneMinute, 12);
                max = get_max_value(chart_data.humidity.oneMinute, 12);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.humidity.oneHour);
                min = get_min_value(chart_data.humidity.oneHour, 12);
                max = get_max_value(chart_data.humidity.oneHour, 12);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.humidity.oneDay);
                min = get_min_value(chart_data.humidity.oneDay, 24);
                max = get_max_value(chart_data.humidity.oneDay, 24);
                break;
            }
            min = min < 100 ? 0 : min - 100;
            max = max > 900 ? 1000 : max + 100;
            break;
        case CHART_TYPE_VOC:
            switch (current_time_frame)
            {
            case TIME_FRAME_1MIN:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.voc.oneMinute);
                min = get_min_value(chart_data.voc.oneMinute, 12);
                max = get_max_value(chart_data.voc.oneMinute, 12);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.voc.oneHour);
                min = get_min_value(chart_data.voc.oneHour, 12);
                max = get_max_value(chart_data.voc.oneHour, 12);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.voc.oneDay);
                min = get_min_value(chart_data.voc.oneDay, 24);
                max = get_max_value(chart_data.voc.oneDay, 24);
                break;
            }
            min = min < 50 ? 0 : min - 50;
            max = max > 500 ? 500 : max + 50;
            break;
        }
        mid = (max + min) / 2;
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max); // 动态调整范围

        // 设置Y轴的刻度文字
        switch (current_chart_type)
        {
        // 温度和湿度都是浮点数，转成了整数计算，最终显示时再转换回去
        case CHART_TYPE_TEMPERATURE:
        case CHART_TYPE_HUMIDITY:
            sprintf(str0, "%.1f", min / 10.0);
            sprintf(str1, "%.1f", mid / 10.0);
            sprintf(str2, "%.1f", max / 10.0);
            break;
        case CHART_TYPE_VOC:
        case CHART_TYPE_CO2:
            sprintf(str0, "%ld", min);
            sprintf(str1, "%ld", mid);
            sprintf(str2, "%ld", max);
            break;
        }
        // 设置Y轴显示范围
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
        _lock_release(&lvgl_api_lock);
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000)); // 每5秒或收到任务通知时更新图表
    }
}

void get_data_task(void *arg)
{
    uint16_t cnt = 0;
    while (1)
    {
        chart_data.co2.oneMinute[12] = stcc4.co2Concentration;
        chart_data.temperature.oneMinute[12] = (int32_t)(stcc4.temperature * 10);
        chart_data.humidity.oneMinute[12] = (int32_t)(stcc4.relativeHumidity * 10);
        chart_data.voc.oneMinute[12] = voc_index;
        cnt++;
        // 每间隔5秒钟，一分钟数据前移
        for (int i = 0; i < 12; i++)
        {
            chart_data.co2.oneMinute[i] = chart_data.co2.oneMinute[i + 1];
            chart_data.temperature.oneMinute[i] = chart_data.temperature.oneMinute[i + 1];
            chart_data.humidity.oneMinute[i] = chart_data.humidity.oneMinute[i + 1];
            chart_data.voc.oneMinute[i] = chart_data.voc.oneMinute[i + 1];
        }
        // 每间隔5分钟，一小时的数据前移
        if (cnt % 60 == 0)
        {
            chart_data.co2.oneHour[12] = chart_data.co2.oneMinute[12];
            chart_data.temperature.oneHour[12] = chart_data.temperature.oneMinute[12];
            chart_data.humidity.oneHour[12] = chart_data.humidity.oneMinute[12];
            chart_data.voc.oneHour[12] = chart_data.voc.oneMinute[12];
            for (int i = 0; i < 12; i++)
            {
                chart_data.co2.oneHour[i] = chart_data.co2.oneHour[i + 1];
                chart_data.temperature.oneHour[i] = chart_data.temperature.oneHour[i + 1];
                chart_data.humidity.oneHour[i] = chart_data.humidity.oneHour[i + 1];
                chart_data.voc.oneHour[i] = chart_data.voc.oneHour[i + 1];
            }
        }
        // 每间隔1小时，一天数据前移
        if (cnt % 720 == 0)
        {
            cnt = 0;
            chart_data.co2.oneDay[24] = chart_data.co2.oneHour[12];
            chart_data.temperature.oneDay[24] = chart_data.temperature.oneHour[12];
            chart_data.humidity.oneDay[24] = chart_data.humidity.oneHour[12];
            chart_data.voc.oneDay[24] = chart_data.voc.oneHour[12];
            for (int i = 0; i < 24; i++)
            {
                chart_data.co2.oneDay[i] = chart_data.co2.oneDay[i + 1];
                chart_data.temperature.oneDay[i] = chart_data.temperature.oneDay[i + 1];
                chart_data.humidity.oneDay[i] = chart_data.humidity.oneDay[i + 1];
                chart_data.voc.oneDay[i] = chart_data.voc.oneDay[i + 1];
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // 每5秒获取一次数据
    }
}

void create_chart()
{
    lv_obj_t *main_cont = lv_obj_create(guider_ui.data_chart_screen);
    lv_obj_set_pos(main_cont, 10, 38);
    lv_obj_set_size(main_cont, 460, 180);
    lv_obj_center(main_cont);
    // 创建透明容器，让图表能够左右滑动
    lv_obj_t *wrapper = lv_obj_create(main_cont);
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_flex_flow(wrapper, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(wrapper, lv_pct(90), lv_pct(100));
    lv_obj_set_pos(wrapper, lv_pct(10), 0);

    chart = lv_chart_create(wrapper);
    lv_obj_set_width(chart, lv_pct(100));
    lv_obj_set_flex_grow(chart, 1);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(chart, 3, 5);
    lv_chart_set_point_count(chart, 12);                        // 绘制点数
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100); // 范围
    ser1 = lv_chart_add_series(chart, lv_color_hex(0x000000), LV_CHART_AXIS_PRIMARY_Y);

    // 设置X轴刻度
    scale_bottom = lv_scale_create(wrapper);
    lv_scale_set_mode(scale_bottom, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
    lv_obj_set_size(scale_bottom, lv_pct(100), lv_pct(15));
    lv_scale_set_total_tick_count(scale_bottom, 12);
    lv_scale_set_major_tick_every(scale_bottom, 2); // 隔一个放一个刻度
    lv_obj_set_style_pad_hor(scale_bottom, lv_chart_get_first_point_center_offset(chart), 0);
    update_chart_x_scale_text();

    // 设置Y轴刻度
    scale_left = lv_scale_create(main_cont);
    lv_scale_set_mode(scale_left, LV_SCALE_MODE_VERTICAL_LEFT);
    lv_obj_set_size(scale_left, lv_pct(10), lv_pct(85));
    lv_obj_set_pos(scale_left, 0, 0);
    lv_scale_set_total_tick_count(scale_left, 3);
    lv_scale_set_major_tick_every(scale_left, 1);
    lv_scale_set_text_src(scale_left, y_scale);
    
    // 修改数据显示页的标题
    switch (current_chart_type)
    {
    case CHART_TYPE_CO2:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "CO2 历史数据(ppm)");
        break;
    case CHART_TYPE_TEMPERATURE:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "温度 历史数据(℃)");
        break;
    case CHART_TYPE_HUMIDITY:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "湿度 历史数据(%)");
        break;
    case CHART_TYPE_VOC:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "VOC指数 历史数据");
        break;
    }
    // 创建更新图表的任务
    xTaskCreate(update_chart_task, "update_chart_task", 16 * 1024, NULL, 5, &update_chart_task_handle);
    // TODO: 改图表样式
}

void delete_chart()
{
    if (update_chart_task_handle)
    {
        vTaskDelete(update_chart_task_handle);
        update_chart_task_handle = NULL;
    }
    if (chart)
    {
        lv_obj_del(chart);
        chart = NULL;
    }
}