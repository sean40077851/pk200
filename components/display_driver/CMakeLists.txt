idf_component_register(
    SRCS "display_driver.c"
    INCLUDE_DIRS "include"
    REQUIRES
        driver
        esp_system
        esp_common
        spi_flash
        lvgl
        touch_driver
)
