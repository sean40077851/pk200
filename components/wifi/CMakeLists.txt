idf_component_register(
    SRCS "wifi.c"
    INCLUDE_DIRS "include"
    REQUIRES
    config_manager
    esp_wifi
    esp_event
    esp_system
    nvs_flash
    esp_timer
)
