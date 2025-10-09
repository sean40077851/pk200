idf_component_register(
    SRCS "ota.c"
    INCLUDE_DIRS "include"
    REQUIRES
        esp_http_client
        app_update
        esp_system
        wifi
)