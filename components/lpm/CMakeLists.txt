idf_component_register(SRCS "lpm.c"
                    PRIV_REQUIRES "esp_pm" "esp_driver_gpio" "backlight"
                    INCLUDE_DIRS "include")
