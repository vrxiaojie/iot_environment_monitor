#include "RTOS_tasks.h"
#include "data_chart.h"
#include "stcc4.h"
#include "sgp4x.h"

extern STCC4_t stcc4;
extern int32_t voc_index;

void get_data_task(void *arg)
{
    uint16_t cnt = 0;
    while (1)
    {
        if (stcc4_task_handle != NULL || spg4x_task_handle != NULL)
        {
            xTaskNotifyGive(stcc4_task_handle);
            xTaskNotifyGive(spg4x_task_handle);
        }
        cnt++;
        // 每间隔5秒钟，一分钟数据前移
        for (int i = 0; i < 11; i++)
        {
            chart_data.co2.oneMinute[i] = chart_data.co2.oneMinute[i + 1];
            chart_data.temperature.oneMinute[i] = chart_data.temperature.oneMinute[i + 1];
            chart_data.humidity.oneMinute[i] = chart_data.humidity.oneMinute[i + 1];
            chart_data.voc.oneMinute[i] = chart_data.voc.oneMinute[i + 1];
        }
        chart_data.co2.oneMinute[11] = stcc4.co2Concentration;
        chart_data.temperature.oneMinute[11] = (int32_t)(stcc4.temperature * 10);
        chart_data.humidity.oneMinute[11] = (int32_t)(stcc4.relativeHumidity * 10);
        chart_data.voc.oneMinute[11] = voc_index;
        if (chart_data.oneMinute_cnt < 12)
        {
            chart_data.oneMinute_cnt++;
        }

        // 每间隔5分钟，一小时的数据前移
        if (cnt % 60 == 0)
        {
            for (int i = 0; i < 11; i++)
            {
                chart_data.co2.oneHour[i] = chart_data.co2.oneHour[i + 1];
                chart_data.temperature.oneHour[i] = chart_data.temperature.oneHour[i + 1];
                chart_data.humidity.oneHour[i] = chart_data.humidity.oneHour[i + 1];
                chart_data.voc.oneHour[i] = chart_data.voc.oneHour[i + 1];
            }
            chart_data.co2.oneHour[11] = chart_data.co2.oneMinute[11];
            chart_data.temperature.oneHour[11] = chart_data.temperature.oneMinute[11];
            chart_data.humidity.oneHour[11] = chart_data.humidity.oneMinute[11];
            chart_data.voc.oneHour[11] = chart_data.voc.oneMinute[11];
            if (chart_data.oneHour_cnt < 12)
            {
                chart_data.oneHour_cnt++;
            }
        }
        // 每间隔1小时，一天数据前移
        if (cnt % 720 == 0)
        {
            cnt = 0;
            for (int i = 0; i < 23; i++)
            {
                chart_data.co2.oneDay[i] = chart_data.co2.oneDay[i + 1];
                chart_data.temperature.oneDay[i] = chart_data.temperature.oneDay[i + 1];
                chart_data.humidity.oneDay[i] = chart_data.humidity.oneDay[i + 1];
                chart_data.voc.oneDay[i] = chart_data.voc.oneDay[i + 1];
            }
            chart_data.co2.oneDay[23] = chart_data.co2.oneHour[11];
            chart_data.temperature.oneDay[23] = chart_data.temperature.oneHour[11];
            chart_data.humidity.oneDay[23] = chart_data.humidity.oneHour[11];
            chart_data.voc.oneDay[23] = chart_data.voc.oneHour[11];
            if (chart_data.oneDay_cnt < 24)
            {
                chart_data.oneDay_cnt++;
            }
        }
        if (update_chart_task_handle != NULL)
            xTaskNotifyGive(update_chart_task_handle);
        vTaskDelay(pdMS_TO_TICKS(5000)); // 每5秒获取一次数据
    }
}