idf_component_register(SRCS "bat_adc.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_adc" "adc_battery_estimation"
                    )
