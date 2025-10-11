idf_component_register(SRCS "ota.c"
                    REQUIRES esp_http_client app_update esp_netif json mbedtls
                    PRIV_REQUIRES "wifi" "rgb_lcd" "nvs_helper"
                    INCLUDE_DIRS "include")
