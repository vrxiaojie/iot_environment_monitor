/*
本组件修改自ESP-IDF组件 ningzixi/weather，GitHub仓库https://github.com/NingZiXi/weather，感谢作者开源
*/

#include "weather.h"
#include "esp_log.h"
#include "cJSON.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "lwip/dns.h"
#include "esp_crt_bundle.h"
#include <ctype.h>
#include "zlib.h"
static const char *TAG = "WEATHER";

// 定义安全释放宏
#define SAFE_FREE(ptr)  \
    do                  \
    {                   \
        if (ptr)        \
        {               \
            free(ptr);  \
            ptr = NULL; \
        }               \
    } while (0)

// 内部使用的缓冲区
static char weather_buffer[2048];
static size_t weather_len = 0;

// 用于存储 Content-Encoding 头部值
static char content_encoding_value[32] = {0};

// URL编码函数
static void url_encode(char *dest, const char *src, size_t max_len)
{
    size_t i = 0, j = 0;
    while (src[i] != '\0' && j < max_len - 1)
    {
        unsigned char c = (unsigned char)src[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            dest[j++] = src[i++];
        }
        else
        {
            if (j + 3 >= max_len)
                break; // 额外的安全检查
            snprintf(&dest[j], max_len - j, "%%%02X", c);
            j += 3;
            i++;
        }
    }
    dest[j] = '\0';
}

// HTTP事件处理器
static esp_err_t weather_http_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        // 存储Content-Encoding头
        if (strcasecmp(evt->header_key, "Content-Encoding") == 0)
        {
            strlcpy(content_encoding_value, evt->header_value, sizeof(content_encoding_value));
        }
        break;

    case HTTP_EVENT_ON_DATA:
        if (weather_len + evt->data_len >= sizeof(weather_buffer))
        {
            ESP_LOGE(TAG, "Buffer overflow");
            return ESP_FAIL;
        }
        memcpy(weather_buffer + weather_len, evt->data, evt->data_len);
        weather_len += evt->data_len;
        break;
    default:
        break;
    }
    return ESP_OK;
}

// 解压 gzip 数据
static char *decompress_gzip_data(const char *compressed_data, int compressed_len)
{
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = compressed_len;
    strm.next_in = (Bytef *)compressed_data;

    // 分配一个足够大的缓冲区来存储解压后的数据
    char *uncompressed_data = malloc(compressed_len * 2); // 假设解压后数据不会超过两倍
    if (uncompressed_data == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for uncompressed data");
        return NULL;
    }

    strm.avail_out = compressed_len * 2;
    strm.next_out = (Bytef *)uncompressed_data;

    int ret = inflateInit2(&strm, 16 + MAX_WBITS);
    if (ret != Z_OK)
    {
        ESP_LOGE(TAG, "inflateInit2 failed: %d", ret);
        free(uncompressed_data);
        return NULL;
    }

    ret = inflate(&strm, Z_NO_FLUSH);
    if (ret != Z_STREAM_END)
    {
        ESP_LOGE(TAG, "inflate failed: %d", ret);
        inflateEnd(&strm);
        free(uncompressed_data);
        return NULL;
    }

    inflateEnd(&strm);

    // 确保解压后的数据以 null 结尾
    uncompressed_data[strm.total_out] = '\0';

    return uncompressed_data;
}

// 执行HTTP请求
static char *weather_http_request(const char *url, const char *api_key)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .event_handler = weather_http_handler,
        .timeout_ms = 60000,
        .buffer_size = 3072,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "X-QW-Api-Key", api_key); // 设置api-key

    weather_len = 0;
    memset(weather_buffer, 0, sizeof(weather_buffer));
    memset(content_encoding_value, 0, sizeof(content_encoding_value)); // 清空编码信息

    esp_err_t err = esp_http_client_perform(client); // 执行HTTP请求
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Request failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return NULL;
    }

    if (weather_len >= sizeof(weather_buffer))
    {
        weather_len = sizeof(weather_buffer) - 1;
    }
    weather_buffer[weather_len] = '\0';

    // 检查是否是 gzip 压缩数据
    if (content_encoding_value[0] && strcmp(content_encoding_value, "gzip") == 0)
    {
        ESP_LOGI(TAG, "Data is gzip compressed, decompressing...");
        char *uncompressed_data = decompress_gzip_data(weather_buffer, weather_len);
        if (uncompressed_data)
        {
            esp_http_client_cleanup(client);
            return uncompressed_data;
        }
        else
        {
            esp_http_client_cleanup(client);
            return NULL;
        }
    }

    esp_http_client_cleanup(client);
    return strdup(weather_buffer);
}

// 获取和风天气的城市ID
static char *get_hefeng_location_id(const char *city_name, const char *api_host, const char *api_key)
{
    char url[256];
    char encoded_city[64] = {0};
    url_encode(encoded_city, city_name, sizeof(encoded_city));

    snprintf(url, sizeof(url), "https://%s/geo/v2/city/lookup?&location=%s&number=1",
             api_host, encoded_city);

    char *response = weather_http_request(url, api_key);
    if (!response)
        return NULL;

    cJSON *root = cJSON_Parse(response);
    if (!root)
    {
        free(response);
        return NULL;
    }
    char *location_id = NULL;
    cJSON *code = cJSON_GetObjectItem(root, "code");
    if (code && cJSON_IsString(code) && strcmp(code->valuestring, "200") == 0)
    {
        cJSON *location = cJSON_GetObjectItem(root, "location");
        if (location && cJSON_IsArray(location))
        {
            cJSON *first_item = cJSON_GetArrayItem(location, 0);
            if (first_item)
            {
                cJSON *id = cJSON_GetObjectItem(first_item, "id");
                if (id && cJSON_IsString(id))
                {
                    location_id = strdup(id->valuestring);
                }
            }
        }
    }
    cJSON_Delete(root);
    free(response);
    return location_id;
}

// 解析和风天气
static weather_info_t *parse_hefeng(const char *json)
{
    cJSON *root = cJSON_Parse(json);
    if (!root)
        return NULL;

    weather_info_t *info = calloc(1, sizeof(weather_info_t));
    if (!info)
    {
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *now = cJSON_GetObjectItem(root, "now");
    if (now)
    {
        // 基本天气信息
        info->weather = strdup(cJSON_GetObjectItem(now, "text")->valuestring);
        info->temperature = atof(cJSON_GetObjectItem(now, "temp")->valuestring);
        info->feels_like = atof(cJSON_GetObjectItem(now, "feelsLike")->valuestring);
        info->humidity = atof(cJSON_GetObjectItem(now, "humidity")->valuestring);

        // 风信息
        info->wind_dir = strdup(cJSON_GetObjectItem(now, "windDir")->valuestring);
        info->wind_scale = strdup(cJSON_GetObjectItem(now, "windScale")->valuestring);
        info->wind_speed = atof(cJSON_GetObjectItem(now, "windSpeed")->valuestring);

        // 其他气象数据
        info->precip = atof(cJSON_GetObjectItem(now, "precip")->valuestring);
        info->pressure = atof(cJSON_GetObjectItem(now, "pressure")->valuestring);
        info->visibility = atof(cJSON_GetObjectItem(now, "vis")->valuestring);
        info->cloud = atof(cJSON_GetObjectItem(now, "cloud")->valuestring);
        info->dew_point = atof(cJSON_GetObjectItem(now, "dew")->valuestring);

        // 更新时间
        info->update_time = strdup(cJSON_GetObjectItem(root, "updateTime")->valuestring);
    }

    cJSON_Delete(root);
    return info;
}

weather_info_t *weather_get(weather_config_t *config)
{

    location_info_t *location_info = NULL;
    // 如果配置中指定了位置，直接使用
    if (config->city && strlen(config->city) > 0)
    {
        location_info = calloc(1, sizeof(location_info_t));
        location_info->city = strdup(config->city);
    }

    char url[256];
    weather_info_t *weather_info = NULL;
    char *response = NULL;

    if (!config->api_host || !config->api_key)
    {
        ESP_LOGE(TAG, "HEFENG API host or key is not configured");
        return NULL;
    }
    char *location_id = get_hefeng_location_id(location_info->city, config->api_host, config->api_key);
    if (!location_id)
    {
        ESP_LOGE(TAG, "Failed to get location ID for city: %s", location_info->city);
        location_info_free(location_info);
        return NULL;
    }

    snprintf(url, sizeof(url), "https://%s/v7/weather/now?location=%s",
             config->api_host, location_id);
    response = weather_http_request(url, config->api_key);
    if (response)
    {
        weather_info = parse_hefeng(response);
        free(response);
    }

    free(location_id);
    if (weather_info != NULL)
    {
        weather_info->location_info = location_info; // 保存位置信息
    }
    return weather_info;
}

void weather_print_info(const weather_info_t *info)
{
    if (!info)
        return;

    // 天气符号映射
    const char *weather_icon = "☁️";
    if (info->weather)
    {
        if (strstr(info->weather, "晴"))
            weather_icon = "☀️";
        else if (strstr(info->weather, "雨"))
            weather_icon = "🌧️";
        else if (strstr(info->weather, "雪"))
            weather_icon = "❄️";
        else if (strstr(info->weather, "雷"))
            weather_icon = "⛈️";
        else if (strstr(info->weather, "雾"))
            weather_icon = "🌫️";
        else if (strstr(info->weather, "风"))
            weather_icon = "🌬️";
    }

    printf("\n  🌈 WEATHER REPORT %s\n", weather_icon);
    printf("%s\n", " _ _ _ _____ _____ _____ _____ _____ _____ ");
    printf("%s\n", "| | | |   __|  _  |_   _|  |  |   __| __  |");
    printf("%s\n", "| | | |   __|     | | | |     |   __|    -|");
    printf("%s\n", "|_____|_____|__|__| |_| |__|__|_____|__|__|\n");

    // 位置信息
    if (info->location_info)
    {
        char city[64] = {0};
        if (info->location_info->province && info->location_info->city)
        {
            snprintf(city, sizeof(city), "%s·%s",
                     info->location_info->province,
                     info->location_info->city);
        }
        else if (info->location_info->province)
        {
            snprintf(city, sizeof(city), "%s",
                     info->location_info->province);
        }
        else if (info->location_info->city)
        {
            snprintf(city, sizeof(city), "%s",
                     info->location_info->city);
        }
        printf("📍 位置: %s\n", city);
    }

    // 预格式化带单位的数据
    char temp_str[16] = "", feels_str[16] = "", humidity_str[16] = "";
    char precip_str[16] = "", pressure_str[16] = "", vis_str[16] = "";
    char cloud_str[16] = "", dew_str[16] = "", wspeed_str[16] = "";

    if (info->temperature != 0.0f)
        snprintf(temp_str, sizeof(temp_str), "%.1f℃", info->temperature);
    if (info->feels_like != 0.0f)
        snprintf(feels_str, sizeof(feels_str), "%.1f℃", info->feels_like);
    if (info->humidity != 0.0f)
        snprintf(humidity_str, sizeof(humidity_str), "%.1f%%", info->humidity);
    if (info->wind_speed != 0.0f)
        snprintf(wspeed_str, sizeof(wspeed_str), "%.1fkm/h", info->wind_speed);
    if (info->precip != 0.0f)
        snprintf(precip_str, sizeof(precip_str), "%.1fmm", info->precip);
    if (info->pressure != 0.0f)
        snprintf(pressure_str, sizeof(pressure_str), "%.1fhPa", info->pressure);
    if (info->visibility != 0.0f)
        snprintf(vis_str, sizeof(vis_str), "%.1fkm", info->visibility);
    if (info->cloud != 0.0f)
        snprintf(cloud_str, sizeof(cloud_str), "%.1f%%", info->cloud);
    if (info->dew_point != 0.0f)
        snprintf(dew_str, sizeof(dew_str), "%.1f℃", info->dew_point);

    printf("┌───────────────────────────────────────┐\n");
    if (info->weather)
        printf("│ %s  天气: %-29s │\n", weather_icon, info->weather);
    if (info->temperature != 0.0f)
        printf("│ 🌡️  温度: %-30s │\n", temp_str);
    if (info->feels_like != 0.0f)
        printf("│ 🤒 体感: %-30s │\n", feels_str);
    if (info->humidity != 0.0f)
        printf("│ 💧 湿度: %-28s │\n", humidity_str);
    if (info->wind_dir)
        printf("│ 🍃 风向: %-31s │\n", info->wind_dir);
    if (info->wind_scale)
        printf("│ 💨 风力: %-28s │\n", info->wind_scale);
    if (info->wind_speed != 0.0f)
        printf("│ 🌬️  风速: %-28s │\n", wspeed_str);
    if (info->precip != 0.0f)
        printf("│ 🌧️  降水: %-28s │\n", precip_str);
    if (info->pressure != 0.0f)
        printf("│ ⏲️  气压: %-28s │\n", pressure_str);
    if (info->visibility != 0.0f)
        printf("│ 👁️  能见度: %-26s │\n", vis_str);
    if (info->cloud != 0.0f)
        printf("│ ☁️  云量: %-28s │\n", cloud_str);
    if (info->dew_point != 0.0f)
        printf("│ 💦 露点: %-30s │\n", dew_str);
    printf("└───────────────────────────────────────┘\n");

    if (info->update_time)
        printf("\n🕒 更新时间: %s\n", info->update_time);
    printf("\n");
}

void location_info_free(location_info_t *location_info)
{
    if (!location_info)
        return;

    SAFE_FREE(location_info->province);
    SAFE_FREE(location_info->city);
    SAFE_FREE(location_info);
}

void weather_info_free(weather_info_t *info)
{
    if (!info)
        return;

    // 释放天气信息
    SAFE_FREE(info->weather);
    SAFE_FREE(info->wind_dir);
    SAFE_FREE(info->wind_scale);
    SAFE_FREE(info->update_time);

    // 释放位置信息
    if (info->location_info)
    {
        location_info_free(info->location_info);
    }
    SAFE_FREE(info);
}