# LVGL Component for ESP-IDF

set(LVGL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/lvgl-release-v8.4)

# Collect all LVGL source files
file(GLOB_RECURSE LVGL_SOURCES
    "${LVGL_ROOT_DIR}/src/*.c"
)

# Remove demo and example files if they exist
list(FILTER LVGL_SOURCES EXCLUDE REGEX ".*demo.*")
list(FILTER LVGL_SOURCES EXCLUDE REGEX ".*example.*")

idf_component_register(
    SRCS ${LVGL_SOURCES} "lv_port_mem.c"
    INCLUDE_DIRS
        "${LVGL_ROOT_DIR}"
        "${LVGL_ROOT_DIR}/src"
        "."
    REQUIRES
        driver
        esp_timer
        freertos
        heap
)
