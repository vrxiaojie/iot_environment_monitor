idf_component_register(SRCS "sensirion_gas_index_algorithm.c" "type_utils.c" "sgp4x.c"
                    INCLUDE_DIRS "./include"
                    REQUIRES "esp_driver_i2c" "esp_timer"
                    )
