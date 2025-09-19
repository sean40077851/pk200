# Add sources from ui directory
file(GLOB_RECURSE SRC_UI "ui/*.c")

idf_component_register(SRCS "main.c" ${SRC_UI}
                    INCLUDE_DIRS "." "ui" "ui/components" "ui/screens" "ui/images" "ui/fonts"
                    REQUIRES lvgl display_driver touch_driver nvs_flash wifi esp_event esp_system my_mqtt)  # Added wifi, esp_event, esp_system
