idf_component_register(SRCS "mqtt_user.c"
                    REQUIRES "mqtt"
                    PRIV_REQUIRES "nvs_helper" "stcc4" "sgp4x" "wifi"
                    INCLUDE_DIRS "include")
