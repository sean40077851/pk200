#include <stdio.h>
#include "string.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_log.h"
#include "driver/sdmmc_host.h"

#define MOUNT_POINT "/sdcard"
#define TAG "SD_STORAGE"
#define SDIO_FILELINE_MAX_CHAR_SIZE 1024

sdmmc_card_t *card=NULL;
void sdcard_init(void) {
    esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = 20000;

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1;
    slot_config.clk   = GPIO_NUM_12;
    slot_config.cmd   = GPIO_NUM_13;
    slot_config.d0    = GPIO_NUM_11;
    // slot_config.d3 = GPIO_NUM_27;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    ESP_LOGI(TAG,"Initializing SD card");
    ESP_LOGI(TAG,"Mounting filesystem");
    esp_err_t ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. Set format_if_mount_failed = true to format.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). Check pull-ups/wiring.", esp_err_to_name(ret));
        }
        return;
    }

    ESP_LOGI(TAG, "SD card mounted at %s", MOUNT_POINT);
    sdmmc_card_print_info(stdout, card);  
}

esp_err_t sdio_write_file(const char* path, const char* data){
    ESP_LOGI(TAG,"Opening file %s",path);
    FILE* f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, "%s", data);
    fclose(f);
    ESP_LOGI(TAG, "File written");
    return ESP_OK;
}
esp_err_t sdio_read_file(const char* path, char* buffer, size_t max_len){
    ESP_LOGI(TAG,"Reading file %s",path);
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    fgets(buffer, max_len, f);
    fclose(f);
    ESP_LOGI(TAG, "File read: %s",buffer);
    return ESP_OK;
}
void sdcard_test(void){
    esp_err_t ret;
    const char* tflie = "/sdcard/test.txt";
    char data[SDIO_FILELINE_MAX_CHAR_SIZE];

    snprintf(data, SDIO_FILELINE_MAX_CHAR_SIZE, "SD card test data.\n");
    ret = sdio_write_file(tflie, data);
    if(ret != ESP_OK){
        ESP_LOGE(TAG,"Write file failed");
        return;
    }
    ret = sdio_read_file(tflie, data, SDIO_FILELINE_MAX_CHAR_SIZE);
    if(ret != ESP_OK){
        ESP_LOGE(TAG,"Read file failed");
        return;
    }
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    ESP_LOGI(TAG, "SD card unmounted");

}