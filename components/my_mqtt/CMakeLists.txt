idf_component_register(
    SRCS "my_mqtt.c"
    INCLUDE_DIRS "include"
    REQUIRES lvgl mqtt esp_event esp_system config_manager ota
)