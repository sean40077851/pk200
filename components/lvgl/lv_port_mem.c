#include "esp_heap_caps.h"
#include <stdlib.h>

/**
 * Custom memory allocation function for LVGL
 * Uses PSRAM if available, otherwise internal RAM
 */
void *lv_mem_custom_alloc(size_t size) {
    // Try to allocate from PSRAM first
    void *ptr = heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (ptr == NULL) {
        // If PSRAM allocation fails, use internal RAM
        ptr = heap_caps_malloc(size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    }
    return ptr;
}

/**
 * Custom memory reallocation function for LVGL
 */
void *lv_mem_custom_realloc(void *ptr, size_t size) {
    // Try to reallocate in PSRAM first
    void *new_ptr = heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (new_ptr == NULL && size > 0) {
        // If PSRAM reallocation fails, try internal RAM
        new_ptr = heap_caps_realloc(ptr, size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    }
    return new_ptr;
}
