file(GLOB RTOS_TASK_SRCS "RTOS_tasks/*.c")
file(GLOB UI_SRCS_CUSTOM "ui/custom/*.c")
file(GLOB UI_SRCS_GEN "ui/generated/*.c")
file(GLOB UI_SRCS_IMAGE "ui/generated/images/*.c")
file(GLOB UI_SRCS_FONT "ui/generated/guider_fonts/*.c")
file(GLOB CURRENT_SRCS "./*.c")

idf_component_register( SRCS 
                        ${CURRENT_SRCS}
                        ${RTOS_TASK_SRCS}
                        ${UI_SRCS_CUSTOM}
                        ${UI_SRCS_GEN}
                        ${UI_SRCS_IMAGE}
                        ${UI_SRCS_FONT}

                       PRIV_REQUIRES "esp_lcd" 
                       "stcc4" 
                       "sgp4x" 
                       "bat_adc"
                       INCLUDE_DIRS "./include" "./ui/custom/" "./ui/generated/" "./RTOS_tasks/")
