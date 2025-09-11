#include "display_driver.h"
#include "touch_driver.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "esp_heap_caps.h"
#include <string.h>

static const char *TAG = "DISPLAY_DRIVER";

// LVGL display buffer - Balanced size for performance and SPI limits
#define DISP_BUF_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT / 8)  // 1/8 of screen for optimal balance
static lv_color_t *disp_buf1 = NULL;  // Will be allocated in PSRAM
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

// LVGL input device
static lv_indev_drv_t indev_drv;

// ST7796 Display pins configuration
#define PIN_NUM_MISO    GPIO_NUM_NC  // Not used
#define PIN_NUM_MOSI    GPIO_NUM_5
#define PIN_NUM_CLK     GPIO_NUM_6
#define PIN_NUM_CS      GPIO_NUM_7
#define PIN_NUM_DC      GPIO_NUM_15
#define PIN_NUM_RST     GPIO_NUM_16
#define PIN_NUM_BCKL    GPIO_NUM_4   // Backlight control pin

// SPI configuration
#define SPI_HOST_ID     SPI2_HOST
#define SPI_DMA_CHAN    SPI_DMA_CH_AUTO

static spi_device_handle_t spi_handle;

// LVGL flush callback
static void disp_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    display_flush(area->x1, area->y1, area->x2, area->y2, (uint16_t*)color_p);
    lv_disp_flush_ready(drv);
}

// LVGL touch read callback
static void touch_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    uint16_t x, y;
    bool touched = touch_read(&x, &y);

    if (touched) {
        data->state = LV_INDEV_STATE_PRESSED;
        // Touch coordinates need 180° rotation relative to current mapping
        // Current: LVGL(x,y) = Touch(y, width-1-x)
        // Add 180° rotation: flip both X and Y

        int32_t temp_x = y * DISPLAY_WIDTH / 480;  // Scale Y to LVGL X
        int32_t temp_y = (320 - 1 - x) * DISPLAY_HEIGHT / 320;  // Scale inverted X to LVGL Y

        // Apply 180° rotation: (x,y) -> (width-1-x, height-1-y)
        data->point.x = (DISPLAY_WIDTH - 1) - temp_x;
        data->point.y = (DISPLAY_HEIGHT - 1) - temp_y;

        // Clamp coordinates to LVGL display bounds
        if (data->point.x >= DISPLAY_WIDTH) data->point.x = DISPLAY_WIDTH - 1;
        if (data->point.y >= DISPLAY_HEIGHT) data->point.y = DISPLAY_HEIGHT - 1;
        if (data->point.x < 0) data->point.x = 0;
        if (data->point.y < 0) data->point.y = 0;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// ST7796 Commands
#define ST7796_SWRESET  0x01
#define ST7796_SLPOUT   0x11
#define ST7796_INVOFF   0x20
#define ST7796_INVON    0x21
#define ST7796_DISPOFF  0x28
#define ST7796_DISPON   0x29
#define ST7796_CASET    0x2A
#define ST7796_RASET    0x2B
#define ST7796_RAMWR    0x2C
#define ST7796_MADCTL   0x36
#define ST7796_COLMOD   0x3A

// Send command to display
static void send_cmd(uint8_t cmd) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &cmd;
    gpio_set_level(PIN_NUM_DC, 0);  // Command mode
    ret = spi_device_polling_transmit(spi_handle, &t);
    assert(ret == ESP_OK);
}

// Send data to display
static void send_data(const uint8_t *data, int len) {
    esp_err_t ret;
    if (len == 0) return;
    
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = len * 8;
    t.tx_buffer = data;
    gpio_set_level(PIN_NUM_DC, 1);  // Data mode
    ret = spi_device_polling_transmit(spi_handle, &t);
    assert(ret == ESP_OK);
}

// Send single byte data
static void send_data_byte(uint8_t data) {
    send_data(&data, 1);
}

// Initialize ST7796 display
static void st7796_init(void) {
    // Hardware reset
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    // Software reset
    send_cmd(ST7796_SWRESET);
    vTaskDelay(pdMS_TO_TICKS(150));

    // Sleep out
    send_cmd(ST7796_SLPOUT);
    vTaskDelay(pdMS_TO_TICKS(120));

    // Interface Pixel Format - set first
    send_cmd(ST7796_COLMOD);
    send_data_byte(0x55);  // 16-bit color (RGB565)

    // Memory Data Access Control - IPS panel with rotation=3 (270°/90° CCW)
    send_cmd(ST7796_MADCTL);
    send_data_byte(0xE0);  // MY=1, MX=1, MV=1, ML=0, RGB=0, MH=0 (270° rotation, RGB order, X/Y mirror)

    // Set column address (X coordinates: 0-319)
    send_cmd(ST7796_CASET);
    send_data_byte(0x00);
    send_data_byte(0x00);
    send_data_byte(0x01);  // 319 (0x013F)
    send_data_byte(0x3F);

    // Set row address (Y coordinates: 0-479)
    send_cmd(ST7796_RASET);
    send_data_byte(0x00);
    send_data_byte(0x00);
    send_data_byte(0x01);  // 479 (0x01DF)
    send_data_byte(0xDF);

    // Display inversion on (needed for correct colors)
    send_cmd(ST7796_INVON);

    // Display on
    send_cmd(ST7796_DISPON);
    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_LOGI(TAG, "ST7796 display initialized");
}

esp_err_t display_init(void) {
    esp_err_t ret;

    // Initialize GPIO pins
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_NUM_DC) | (1ULL << PIN_NUM_RST) | (1ULL << PIN_NUM_BCKL),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    // Turn on backlight
    gpio_set_level(PIN_NUM_BCKL, 1);
    ESP_LOGI(TAG, "Backlight turned on");

    // Initialize SPI bus
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * 2 + 8
    };

    ret = spi_bus_initialize(SPI_HOST_ID, &buscfg, SPI_DMA_CHAN);
    ESP_ERROR_CHECK(ret);

    // Initialize SPI device
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 80 * 1000 * 1000,  // 80MHz - Maximum for most LCD controllers
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 7,
        .flags = SPI_DEVICE_NO_DUMMY,
    };

    ret = spi_bus_add_device(SPI_HOST_ID, &devcfg, &spi_handle);
    ESP_ERROR_CHECK(ret);

    // Initialize display
    st7796_init();

    // Allocate LVGL display buffer in PSRAM for better performance
    disp_buf1 = (lv_color_t *)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    if (disp_buf1 == NULL) {
        ESP_LOGE(TAG, "Failed to allocate display buffer in PSRAM, trying internal RAM");
        disp_buf1 = (lv_color_t *)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
        if (disp_buf1 == NULL) {
            ESP_LOGE(TAG, "Failed to allocate display buffer");
            return ESP_ERR_NO_MEM;
        }
    } else {
        ESP_LOGI(TAG, "Display buffer allocated in PSRAM (%d bytes)", DISP_BUF_SIZE * sizeof(lv_color_t));
    }

    // Initialize LVGL display buffer
    lv_disp_draw_buf_init(&draw_buf, disp_buf1, NULL, DISP_BUF_SIZE);

    // Initialize LVGL display driver
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = DISPLAY_WIDTH;
    disp_drv.ver_res = DISPLAY_HEIGHT;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &draw_buf;

    // Register the display driver
    lv_disp_drv_register(&disp_drv);

    // Initialize LVGL input device driver
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read_cb;

    // Register the input device driver
    lv_indev_drv_register(&indev_drv);

    ESP_LOGI(TAG, "ST7796 display initialized");
    ESP_LOGI(TAG, "Touch input device registered");
    ESP_LOGI(TAG, "Display driver initialized successfully");
    return ESP_OK;
}

void display_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t *color_data) {
    // With MADCTL set for 270° rotation, we can directly use LVGL coordinates
    // The LCD controller will handle the rotation for us

    // Set column address
    send_cmd(ST7796_CASET);
    uint8_t col_data[4] = {
        (x1 >> 8) & 0xFF, x1 & 0xFF,
        (x2 >> 8) & 0xFF, x2 & 0xFF
    };
    send_data(col_data, 4);

    // Set row address
    send_cmd(ST7796_RASET);
    uint8_t row_data[4] = {
        (y1 >> 8) & 0xFF, y1 & 0xFF,
        (y2 >> 8) & 0xFF, y2 & 0xFF
    };
    send_data(row_data, 4);

    // Write to RAM
    send_cmd(ST7796_RAMWR);

    // Calculate data size and number of pixels
    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    uint32_t buf_size = size * 2;  // 2 bytes per pixel

    // Convert LVGL RGB565 to BGR565 for display and swap bytes for SPI transmission
    // Try to allocate in PSRAM first for better performance
    uint16_t *swapped_data = (uint16_t *)heap_caps_malloc(buf_size, MALLOC_CAP_SPIRAM);
    if (swapped_data == NULL) {
        swapped_data = (uint16_t *)malloc(buf_size);  // Fallback to internal RAM
    }
    if (swapped_data) {
        for (uint32_t i = 0; i < size; i++) {
            // LVGL RGB565 format: RRRRR GGGGGG BBBBB
            uint16_t color = color_data[i];
            uint16_t r = (color >> 11) & 0x1F;  // Extract red (5 bits)
            uint16_t g = (color >> 5) & 0x3F;   // Extract green (6 bits)
            uint16_t b = color & 0x1F;          // Extract blue (5 bits)

            // Convert to BGR565 format: BBBBB GGGGGG RRRRR
            uint16_t bgr_color = (b << 11) | (g << 5) | r;

            // Swap bytes for SPI transmission (MSB first)
            swapped_data[i] = ((bgr_color & 0xFF) << 8) | ((bgr_color >> 8) & 0xFF);
        }

        // Send pixel data
        gpio_set_level(PIN_NUM_DC, 1);  // Data mode

        // Split large transfers to avoid SPI hardware limits
        const uint32_t max_transfer_size = 32768;  // 32KB max per transfer
        uint32_t remaining = buf_size;
        uint8_t *data_ptr = (uint8_t *)swapped_data;

        while (remaining > 0) {
            uint32_t transfer_size = (remaining > max_transfer_size) ? max_transfer_size : remaining;

            spi_transaction_t t;
            memset(&t, 0, sizeof(t));
            t.length = transfer_size * 8;
            t.tx_buffer = data_ptr;

            esp_err_t ret = spi_device_polling_transmit(spi_handle, &t);
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "SPI transmission failed: %s", esp_err_to_name(ret));
                break;
            }

            data_ptr += transfer_size;
            remaining -= transfer_size;
        }

        free(swapped_data);
    } else {
        ESP_LOGE(TAG, "Failed to allocate memory for color swapping");
    }
}

uint16_t display_get_width(void) {
    return DISPLAY_WIDTH;
}

uint16_t display_get_height(void) {
    return DISPLAY_HEIGHT;
}
