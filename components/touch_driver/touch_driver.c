#include "touch_driver.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "TOUCH_DRIVER";

// FT6336U I2C configuration
#define I2C_MASTER_NUM          I2C_NUM_0
#define I2C_MASTER_FREQ_HZ      400000
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TIMEOUT_MS   1000

// FT6336U I2C address and registers
#define FT6336U_ADDR            0x38
#define FT6336U_REG_TD_STATUS   0x02
#define FT6336U_REG_TOUCH1_XH   0x03
#define FT6336U_REG_TOUCH1_XL   0x04
#define FT6336U_REG_TOUCH1_YH   0x05
#define FT6336U_REG_TOUCH1_YL   0x06

static gpio_num_t rst_pin_num;
static gpio_num_t int_pin_num;

// I2C read function
static esp_err_t i2c_read_reg(uint8_t reg_addr, uint8_t *data, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (FT6336U_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (FT6336U_ADDR << 1) | I2C_MASTER_READ, true);
    if (len > 1) {
        i2c_master_read(cmd, data, len - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + len - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

// I2C write function (currently unused but kept for future use)
__attribute__((unused)) static esp_err_t i2c_write_reg(uint8_t reg_addr, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (FT6336U_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t touch_init(gpio_num_t sda_pin, gpio_num_t scl_pin, gpio_num_t rst_pin, gpio_num_t int_pin) {
    esp_err_t ret;
    
    rst_pin_num = rst_pin;
    int_pin_num = int_pin;

    // Configure I2C
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = scl_pin,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C param config failed");
        return ret;
    }

    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed");
        return ret;
    }

    // Configure GPIO pins
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << rst_pin),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << int_pin);
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    // Reset touch controller
    gpio_set_level(rst_pin, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(rst_pin, 1);
    vTaskDelay(pdMS_TO_TICKS(300));

    // Test I2C communication
    uint8_t test_data;
    ret = i2c_read_reg(FT6336U_REG_TD_STATUS, &test_data, 1);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to communicate with FT6336U");
        return ret;
    }

    ESP_LOGI(TAG, "FT6336U touch controller initialized successfully");
    return ESP_OK;
}

uint8_t touch_get_status(void) {
    uint8_t status = 0;
    esp_err_t ret = i2c_read_reg(FT6336U_REG_TD_STATUS, &status, 1);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read touch status");
        return 0;
    }
    return status & 0x0F;  // Lower 4 bits contain touch point count
}

bool touch_read(uint16_t *x, uint16_t *y) {
    uint8_t touch_status = touch_get_status();
    
    if (touch_status == 0) {
        return false;  // No touch detected
    }

    // Read touch coordinates
    uint8_t touch_data[4];
    esp_err_t ret = i2c_read_reg(FT6336U_REG_TOUCH1_XH, touch_data, 4);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read touch coordinates");
        return false;
    }

    // Extract X and Y coordinates
    *x = ((touch_data[0] & 0x0F) << 8) | touch_data[1];
    *y = ((touch_data[2] & 0x0F) << 8) | touch_data[3];

    return true;
}
