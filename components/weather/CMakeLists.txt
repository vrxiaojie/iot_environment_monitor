idf_component_register(SRCS "weather.c"
                    REQUIRES                     
                    esp_http_client
                    json
                    mbedtls
                    nvs_flash
                    esp_netif
                    esp_event
                    lwip
                    zlib
                    PRIV_REQUIRES nvs_helper
                    INCLUDE_DIRS "include")
