#include "data_chart.h"

chart_data_t chart_data = {0};
extern _lock_t lvgl_api_lock;
time_frame_t current_time_frame = TIME_FRAME_1MIN;
chart_type_t current_chart_type = CHART_TYPE_TEMPERATURE;

TaskHandle_t update_chart_task_handle = NULL;
TaskHandle_t get_data_task_handle = NULL;

static char str0[12], str1[12], str2[12];
static const char *y_scale[] = {str0, str1, str2, NULL}; // Y轴刻度

static lv_obj_t *chart;
static lv_chart_series_t *ser1;
static lv_obj_t *scale_left;
static lv_obj_t *scale_bottom;
static lv_obj_t *wrapper;
static lv_obj_t *value_label = NULL;
static lv_obj_t *main_cont;

static int32_t get_max_value(int32_t *array, int8_t total_size, int8_t size)
{
    int32_t max = array[total_size - 1];
    for (int8_t i = total_size - 1; i > total_size - size; i--)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    return max;
}

static int32_t get_min_value(int32_t *array, int8_t total_size, int8_t size)
{
    int32_t min = array[total_size - 1];
    for (int8_t i = total_size - 1; i > total_size - size; i--)
    {
        if (array[i] < min)
        {
            min = array[i];
        }
    }
    return min;
}

// 图表事件回调函数
static void chart_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    // 当数值改变（即数据点被按下）时
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        // 先删除上一个创建的标签（如果存在）
        if (value_label)
        {
            lv_obj_del(value_label);
            value_label = NULL;
        }

        uint16_t id = lv_chart_get_pressed_point(obj);

        // 获取数据点的坐标
        lv_point_t p;
        lv_chart_get_point_pos_by_id(obj, ser1, id, &p);
        p.x += lv_obj_get_x(main_cont);
        p.y += lv_obj_get_y(main_cont);

        // 获取数据点的数值
        int32_t value = 0;
        int32_t *data_array = NULL;

        // 根据当前图表类型和时间范围，从我们自己的数据源获取值
        switch (current_chart_type)
        {
        case CHART_TYPE_CO2:
            if (current_time_frame == TIME_FRAME_1MIN)
                data_array = chart_data.co2.oneMinute;
            else if (current_time_frame == TIME_FRAME_1HOUR)
                data_array = chart_data.co2.oneHour;
            else
                data_array = chart_data.co2.oneDay;
            break;
        case CHART_TYPE_TEMPERATURE:
            if (current_time_frame == TIME_FRAME_1MIN)
                data_array = chart_data.temperature.oneMinute;
            else if (current_time_frame == TIME_FRAME_1HOUR)
                data_array = chart_data.temperature.oneHour;
            else
                data_array = chart_data.temperature.oneDay;
            break;
        case CHART_TYPE_HUMIDITY:
            if (current_time_frame == TIME_FRAME_1MIN)
                data_array = chart_data.humidity.oneMinute;
            else if (current_time_frame == TIME_FRAME_1HOUR)
                data_array = chart_data.humidity.oneHour;
            else
                data_array = chart_data.humidity.oneDay;
            break;
        case CHART_TYPE_VOC:
            if (current_time_frame == TIME_FRAME_1MIN)
                data_array = chart_data.voc.oneMinute;
            else if (current_time_frame == TIME_FRAME_1HOUR)
                data_array = chart_data.voc.oneHour;
            else
                data_array = chart_data.voc.oneDay;
            break;
        }

        if (data_array)
        {
            value = data_array[id];
        }

        char buf[16];
        // 根据当前图表类型格式化数值字符串
        switch (current_chart_type)
        {
        case CHART_TYPE_TEMPERATURE:
        case CHART_TYPE_HUMIDITY:
            sprintf(buf, "%.1f", value / 10.0);
            break;
        case CHART_TYPE_VOC:
        case CHART_TYPE_CO2:
            sprintf(buf, "%ld", value);
            break;
        }

        // 创建标签来显示数值
        value_label = lv_label_create(main_cont);
        lv_label_set_text(value_label, buf);
        lv_obj_set_style_bg_color(value_label, lv_color_hex(0x000000), 0);
        lv_obj_set_style_bg_opa(value_label, LV_OPA_50, 0);
        lv_obj_set_style_text_color(value_label, lv_color_white(), 0);
        lv_obj_set_style_pad_all(value_label, 2, 0);
        lv_obj_set_style_radius(value_label, 2, 0);
        lv_obj_set_pos(value_label, p.x + 20, p.y - 80); // 将标签放置在数据点的上方
    }
    else if (code == LV_EVENT_RELEASED)
    {
        if (value_label)
        {
            lv_obj_del(value_label);
            value_label = NULL;
        }
    }
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
    struct tm timeinfo;
    time_t now;
    char date_buffer[10], time_buffer[10], data_buffer[10];
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
                min = get_min_value(chart_data.co2.oneMinute, 12, chart_data.oneMinute_cnt);
                max = get_max_value(chart_data.co2.oneMinute, 12, chart_data.oneMinute_cnt);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.co2.oneHour);
                min = get_min_value(chart_data.co2.oneHour, 12, chart_data.oneHour_cnt);
                max = get_max_value(chart_data.co2.oneHour, 12, chart_data.oneHour_cnt);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.co2.oneDay);
                min = get_min_value(chart_data.co2.oneDay, 24, chart_data.oneDay_cnt);
                max = get_max_value(chart_data.co2.oneDay, 24, chart_data.oneDay_cnt);
                break;
            }
            // 取100的倍数
            min = min < 200 ? 0 : (min - 200 - (min % 100));
            max = max + 200 - (max % 100);
            break;
        case CHART_TYPE_TEMPERATURE:
            switch (current_time_frame)
            {
            case TIME_FRAME_1MIN:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.temperature.oneMinute);
                min = get_min_value(chart_data.temperature.oneMinute, 12, chart_data.oneMinute_cnt);
                max = get_max_value(chart_data.temperature.oneMinute, 12, chart_data.oneMinute_cnt);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.temperature.oneHour);
                min = get_min_value(chart_data.temperature.oneHour, 12, chart_data.oneHour_cnt);
                max = get_max_value(chart_data.temperature.oneHour, 12, chart_data.oneHour_cnt);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.temperature.oneDay);
                min = get_min_value(chart_data.temperature.oneDay, 24, chart_data.oneDay_cnt);
                max = get_max_value(chart_data.temperature.oneDay, 24, chart_data.oneDay_cnt);
                break;
            }
            // 取整
            min = min - 10 - (min % 10);
            max = max + 10 - (max % 10);
            break;
        case CHART_TYPE_HUMIDITY:
            switch (current_time_frame)
            {
            case TIME_FRAME_1MIN:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.humidity.oneMinute);
                min = get_min_value(chart_data.humidity.oneMinute, 12, chart_data.oneMinute_cnt);
                max = get_max_value(chart_data.humidity.oneMinute, 12, chart_data.oneMinute_cnt);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.humidity.oneHour);
                min = get_min_value(chart_data.humidity.oneHour, 12, chart_data.oneHour_cnt);
                max = get_max_value(chart_data.humidity.oneHour, 12, chart_data.oneHour_cnt);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.humidity.oneDay);
                min = get_min_value(chart_data.humidity.oneDay, 24, chart_data.oneDay_cnt);
                max = get_max_value(chart_data.humidity.oneDay, 24, chart_data.oneDay_cnt);
                break;
            }
            // 取10的倍数
            min = min < 100 ? 0 : min - min % 100;
            max = max > 800 ? 1000 : (max + 100 - (max % 100));
            break;
        case CHART_TYPE_VOC:
            switch (current_time_frame)
            {
            case TIME_FRAME_1MIN:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.voc.oneMinute);
                min = get_min_value(chart_data.voc.oneMinute, 12, chart_data.oneMinute_cnt);
                max = get_max_value(chart_data.voc.oneMinute, 12, chart_data.oneMinute_cnt);
                break;
            case TIME_FRAME_1HOUR:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.voc.oneHour);
                min = get_min_value(chart_data.voc.oneHour, 12, chart_data.oneHour_cnt);
                max = get_max_value(chart_data.voc.oneHour, 12, chart_data.oneHour_cnt);
                break;
            case TIME_FRAME_1DAY:
                lv_chart_set_ext_y_array(chart, ser1, chart_data.voc.oneDay);
                min = get_min_value(chart_data.voc.oneDay, 24, chart_data.oneDay_cnt);
                max = get_max_value(chart_data.voc.oneDay, 24, chart_data.oneDay_cnt);
                break;
            }
            min = min < 50 ? 0 : (min - 50 - (min % 10));
            max = max + 50 - (max % 10);
            break;
        }
        mid = (max + min) / 2;

        // 设置Y轴显示范围，当该时间跨度的数据量为0时，设置一个较大的y轴范围，实现不显示该图表的效果
        switch (current_time_frame)
        {
        case TIME_FRAME_1MIN:
            if (chart_data.oneMinute_cnt > 0)
            {
                lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
            }
            else
            {
                lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -1000, -999);
            }
            break;
        case TIME_FRAME_1HOUR:
            if (chart_data.oneHour_cnt > 0)
            {
                lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
            }
            else
            {
                lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -1000, -999);
            }
            break;
        case TIME_FRAME_1DAY:
            if (chart_data.oneDay_cnt > 0)
            {
                lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
            }
            else
            {
                lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -1000, -999);
            }
            break;
        }

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

        lv_chart_refresh(chart); // 通知图表重新渲染

        // 更新数据显示页面的数据标签、时间标签
        switch (current_chart_type)
        {
        case CHART_TYPE_CO2:
            sprintf(data_buffer, "%ld ppm", chart_data.co2.oneMinute[11]);
            lv_label_set_text(guider_ui.data_chart_screen_data_label, data_buffer);
            break;
        case CHART_TYPE_TEMPERATURE:
            sprintf(data_buffer, "%.1f C", chart_data.temperature.oneMinute[11] / 10.0);
            lv_label_set_text(guider_ui.data_chart_screen_data_label, data_buffer);
            break;
        case CHART_TYPE_HUMIDITY:
            sprintf(data_buffer, "%.1f %%", chart_data.humidity.oneMinute[11] / 10.0);
            lv_label_set_text(guider_ui.data_chart_screen_data_label, data_buffer);
            break;
        case CHART_TYPE_VOC:
            sprintf(data_buffer, "%ld", chart_data.voc.oneMinute[11]);
            lv_label_set_text(guider_ui.data_chart_screen_data_label, data_buffer);
        }
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(date_buffer, sizeof(date_buffer), "%m-%d", &timeinfo);
        strftime(time_buffer, sizeof(time_buffer), "%H:%M", &timeinfo);
        lv_label_set_text(guider_ui.data_chart_screen_date_label, date_buffer);
        lv_label_set_text(guider_ui.data_chart_screen_time_label, time_buffer);
        _lock_release(&lvgl_api_lock);
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000)); // 每5秒或收到任务通知时更新图表
    }
}

void create_chart()
{
    main_cont = lv_obj_create(guider_ui.data_chart_screen);
    lv_obj_set_pos(main_cont, 10, 38);
    lv_obj_set_size(main_cont, 460, 180);
    lv_obj_center(main_cont);
    lv_obj_set_style_bg_color(main_cont, lv_color_hex(0x5B5B5B), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(main_cont, LV_OPA_40, LV_PART_MAIN);
    lv_obj_set_style_border_width(main_cont, 0, LV_PART_MAIN);

    wrapper = lv_obj_create(main_cont);
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_flex_flow(wrapper, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(wrapper, lv_pct(90), lv_pct(100));
    lv_obj_set_pos(wrapper, lv_pct(10), 0);
    lv_obj_set_style_bg_color(wrapper, lv_color_hex(0x5B5B5B), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(wrapper, LV_OPA_0, LV_PART_MAIN);
    lv_obj_remove_flag(wrapper, LV_OBJ_FLAG_SCROLLABLE);

    chart = lv_chart_create(wrapper);
    lv_obj_set_width(chart, lv_pct(100));
    lv_obj_set_flex_grow(chart, 1);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(chart, 3, 5);
    lv_chart_set_point_count(chart, 12);                        // 绘制点数
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100); // 范围
    ser1 = lv_chart_add_series(chart, lv_color_hex(0x409eff), LV_CHART_AXIS_PRIMARY_Y);
    // 添加事件回调
    lv_obj_add_event_cb(chart, chart_event_cb, LV_EVENT_ALL, NULL);
    // 设置主区域样式
    lv_obj_set_style_bg_color(chart, lv_color_hex(0x5B5B5B), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(chart, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_pad_left(chart, 15, LV_PART_MAIN);
    lv_obj_set_style_border_width(chart, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(chart, lv_color_hex(0x5E5E5E), LV_PART_MAIN);
    lv_obj_set_style_line_color(chart, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_remove_flag(chart, LV_OBJ_FLAG_SCROLLABLE);

    // 设置数据点样式
    lv_obj_set_style_bg_color(chart, lv_color_hex(0x409eff), LV_PART_INDICATOR);
    lv_obj_set_style_size(chart, 4, 4, LV_PART_INDICATOR);
    lv_obj_set_style_radius(chart, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);

    // 设置X轴刻度
    scale_bottom = lv_scale_create(wrapper);
    lv_scale_set_mode(scale_bottom, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
    lv_obj_set_size(scale_bottom, lv_pct(100), lv_pct(15));
    lv_scale_set_total_tick_count(scale_bottom, 12);
    lv_scale_set_major_tick_every(scale_bottom, 2); // 隔一个放一个刻度
    lv_obj_set_style_pad_hor(scale_bottom, lv_chart_get_first_point_center_offset(chart), 0);
    lv_obj_set_style_line_color(scale_bottom, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_color(scale_bottom, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
    lv_obj_set_style_line_color(scale_bottom, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
    lv_obj_set_style_line_color(scale_bottom, lv_color_hex(0xFFFFFF), LV_PART_ITEMS);
    update_chart_x_scale_text();

    // 设置Y轴刻度
    scale_left = lv_scale_create(main_cont);
    lv_scale_set_mode(scale_left, LV_SCALE_MODE_VERTICAL_LEFT);
    lv_obj_set_size(scale_left, lv_pct(10), lv_pct(85));
    lv_obj_set_pos(scale_left, 0, 0);
    lv_scale_set_total_tick_count(scale_left, 3);
    lv_scale_set_major_tick_every(scale_left, 1);
    lv_scale_set_text_src(scale_left, y_scale);
    lv_obj_set_style_line_color(scale_left, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_color(scale_left, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
    lv_obj_set_style_line_color(scale_left, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
    lv_obj_set_style_line_color(scale_left, lv_color_hex(0xFFFFFF), LV_PART_ITEMS);

    // 修改数据显示页的标题
    switch (current_chart_type)
    {
    case CHART_TYPE_CO2:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "CO2 历史数据");
        break;
    case CHART_TYPE_TEMPERATURE:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "温度 历史数据");
        break;
    case CHART_TYPE_HUMIDITY:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "湿度 历史数据");
        break;
    case CHART_TYPE_VOC:
        lv_label_set_text(guider_ui.data_chart_screen_title_label, "VOC指数 历史数据");
        break;
    }
    // 创建更新图表的任务
    xTaskCreateWithCaps(update_chart_task, "update_chart_task", 16 * 1024, NULL, 5, &update_chart_task_handle, MALLOC_CAP_SPIRAM);
}

void delete_chart()
{
    if (update_chart_task_handle != NULL)
    {
        vTaskDelete(update_chart_task_handle);
        vPortFree(update_chart_task_handle);
        update_chart_task_handle = NULL;
    }
    if (chart)
    {
        lv_obj_del(chart);
        chart = NULL;
    }
    if (scale_bottom)
    {
        lv_obj_del(scale_bottom);
        scale_bottom = NULL;
    }
    if (scale_left)
    {
        lv_obj_del(scale_left);
        scale_left = NULL;
    }
    if (wrapper)
    {
        lv_obj_del(wrapper);
        wrapper = NULL;
    }
    if (main_cont)
    {
        lv_obj_del(main_cont);
        main_cont = NULL;
    }
    if (value_label)
    {
        lv_obj_del(value_label);
        value_label = NULL;
    }
}