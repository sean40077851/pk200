#include "ota.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "stdlib.h"
#include "wifi.h"
#include "cJSON.h"
#include "../config_manager/include/config_manager.h"
#define TAG "OTA"
#define OTA_TASK_STACK_SIZE 8192
#define OTA_TASK_PRIORITY   5
#define OTA_URL_MAX_LEN     256

typedef struct {
    char url[OTA_URL_MAX_LEN];
} ota_task_param_t;

static ota_status_callback_t s_status_callback = NULL;
static TaskHandle_t s_ota_task_handle = NULL;
static bool s_ota_in_progress = false;

static void ota_report_status(ota_status_t status, esp_err_t error)
{
    if (s_status_callback) {
        s_status_callback(status, error);
    }
}

static esp_err_t ota_download_and_install(const char *url)
{
    esp_http_client_config_t http_cfg = {
        .url = url,
        .timeout_ms = 10000,
        .keep_alive_enable = true,
        .skip_cert_common_name_check = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&http_cfg);
    if (!client) {
        ESP_LOGE(TAG, "Failed to init HTTP client");
        return ESP_FAIL;
    }

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return err;
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) {
        ESP_LOGW(TAG, "Content length unknown or invalid (%d)", content_length);
    } else {
        ESP_LOGI(TAG, "Content length: %d bytes", content_length);
    }

    const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
    if (!update_partition) {
        ESP_LOGE(TAG, "No OTA partition found");
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%lx",
             update_partition->subtype, update_partition->address);

    esp_ota_handle_t ota_handle = 0;
    err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return err;
    }

    uint8_t *buffer = malloc(4096);
    if (!buffer) {
        ESP_LOGE(TAG, "Failed to allocate OTA buffer");
        esp_ota_end(ota_handle);
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return ESP_ERR_NO_MEM;
    }

    int total_written = 0;
    while (1) {
        int data_read = esp_http_client_read(client, (char *)buffer, 4096);
        if (data_read < 0) {
            ESP_LOGE(TAG, "Error reading data from stream");
            err = ESP_FAIL;
            break;
        }

        if (data_read == 0) {
            ESP_LOGI(TAG, "Download complete (%d bytes)", total_written);
            break;
        }

        err = esp_ota_write(ota_handle, buffer, data_read);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "esp_ota_write failed (%s)", esp_err_to_name(err));
            break;
        }

        total_written += data_read;
        if (content_length > 0) {
            int progress = (total_written * 100) / content_length;
            ESP_LOGI(TAG, "OTA progress: %d%%", progress);
        }
    }

    free(buffer);
    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    if (err != ESP_OK) {
        esp_ota_end(ota_handle);
        return err;
    }

    err = esp_ota_end(ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_end failed (%s)", esp_err_to_name(err));
        return err;
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "OTA update applied successfully");
    return ESP_OK;
}

static void ota_task(void *param)
{
    ota_task_param_t *task_param = (ota_task_param_t *)param;
    char url[OTA_URL_MAX_LEN];
    strlcpy(url, task_param->url, sizeof(url));
    free(task_param);

    ESP_LOGI(TAG, "Starting OTA update from %s", url);
    ota_report_status(OTA_STATUS_IN_PROGRESS, ESP_OK);

    esp_err_t result = ota_download_and_install(url);
    if (result == ESP_OK) {
        ota_report_status(OTA_STATUS_SUCCESS, ESP_OK);
        ESP_LOGI(TAG, "Restarting system in 2 seconds...");
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
    } else {
        ota_report_status(OTA_STATUS_FAILED, result);
        ESP_LOGE(TAG, "OTA update failed: %s", esp_err_to_name(result));
    }

    s_ota_in_progress = false;
    s_ota_task_handle = NULL;
    vTaskDelete(NULL);
}

void ota_init(void)
{
    s_status_callback = NULL;
    s_ota_task_handle = NULL;
    s_ota_in_progress = false;
    ESP_LOGI(TAG, "OTA module initialized");
}

void ota_register_status_callback(ota_status_callback_t callback)
{
    s_status_callback = callback;
}

bool ota_is_in_progress(void)
{
    return s_ota_in_progress;
}

esp_err_t ota_start_async(const char *url)
{
    if (!url || strlen(url) == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!wifi_is_connected()) {
        ESP_LOGW(TAG, "Cannot start OTA - WiFi not connected");
        return ESP_ERR_INVALID_STATE;
    }

    if (s_ota_in_progress) {
        ESP_LOGW(TAG, "OTA already in progress");
        return ESP_ERR_INVALID_STATE;
    }

    size_t url_len = strnlen(url, OTA_URL_MAX_LEN);
    if (url_len == OTA_URL_MAX_LEN) {
        ESP_LOGE(TAG, "OTA URL too long");
        return ESP_ERR_INVALID_SIZE;
    }

    ota_task_param_t *task_param = malloc(sizeof(ota_task_param_t));
    if (!task_param) {
        return ESP_ERR_NO_MEM;
    }

    memset(task_param, 0, sizeof(ota_task_param_t));
    memcpy(task_param->url, url, url_len);
    task_param->url[url_len] = '\0';

    BaseType_t xReturned = xTaskCreate(ota_task, "ota_task",
                                       OTA_TASK_STACK_SIZE, task_param,
                                       OTA_TASK_PRIORITY, &s_ota_task_handle);
    if (xReturned != pdPASS) {
        free(task_param);
        ESP_LOGE(TAG, "Failed to create OTA task");
        return ESP_FAIL;
    }

    s_ota_in_progress = true;
    ESP_LOGI(TAG, "OTA task created successfully");
    return ESP_OK;
}
void check_firmware_version(void)
{
    const char *current_ver = g_device_config.fw_version;
    const char *version_url = "http://27.105.113.156:1577/version.json";
    ESP_LOGI(TAG, "Checking firmware version via HTTP...");

    esp_http_client_config_t config = {
        .url = version_url,
        .timeout_ms = 10000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) return;

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return;
    }

    int status = esp_http_client_get_status_code(client);
    int content_len = esp_http_client_get_content_length(client);
    ESP_LOGI(TAG, "HTTP Status: %d, Content length: %d", status, content_len);

    char buffer[512];
    int data_read = 0;

    esp_err_t err_open = esp_http_client_open(client, 0);
    if (err_open != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err_open));
        esp_http_client_cleanup(client);
        return;
    }

    data_read = esp_http_client_read(client, buffer, sizeof(buffer) - 1);
    if (data_read <= 0) {
        ESP_LOGE(TAG, "No version data read");
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return;
    }

    buffer[data_read] = '\0';
    ESP_LOGI(TAG, "Version JSON: %s", buffer);
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        ESP_LOGE(TAG, "JSON parse error");
        esp_http_client_cleanup(client);
        return;
    }
    cJSON *ver = cJSON_GetObjectItem(json, "latest_version");
    cJSON *url = cJSON_GetObjectItem(json, "url");
    cJSON *update_flag=cJSON_GetObjectItem(json,"update");
    if (!cJSON_IsBool(update_flag) || !cJSON_IsTrue(update_flag))
    {
        ESP_LOGI(TAG, "Update flag is false or missing, skipping OTA");
        cJSON_Delete(json);
        esp_http_client_cleanup(client);
        return;
    }
    if (cJSON_IsString(ver) && cJSON_IsString(url)) {
        if (strcmp(current_ver, ver->valuestring) != 0) {
            ESP_LOGW(TAG, "New version %s available, starting OTA...", ver->valuestring);
            ota_start_async(url->valuestring);
        } else {
            ESP_LOGI(TAG, "Firmware is up to date (%s)", current_ver);
        }

    cJSON_Delete(json);
    esp_http_client_cleanup(client);
}
}