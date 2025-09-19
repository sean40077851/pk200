#include <stdio.h>
#include "mqtt_client.h"
#include "esp_log.h"
#include "my_mqtt.h"

#define TAG             "MQTT"
#define MQTT_ADDRESS    "mqtt://27.105.113.156"  
#define MQTT_USER       "test"  
#define MQTT_PASSWORD   "test123"
#define MQTT_CLIENT_ID  "mqttx_9521a927"

#define MQTT_SUB_TOPIC  "test/esp_topic"
#define MQTT_PUB_TOPIC  "test/mqtt_topic"
esp_mqtt_client_handle_t mqtt_handle = NULL;

void mqtt_eveny_callback(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {

    // 將 event_data 轉型成 MQTT 事件結構指標，方便存取事件資訊
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;

    // 根據不同事件代號 (event_id) 來處理
    switch ((esp_mqtt_event_id_t)event_id) {

        // 成功連線到 MQTT Broker
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            // 成功連線後，馬上訂閱一個 topic
            esp_mqtt_client_subscribe(mqtt_handle, MQTT_SUB_TOPIC, 1);
            break;

        // 與 MQTT Broker 斷線
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        // 成功訂閱 topic
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // 成功訂閱後，測試發佈一則訊息到 MQTT_PUB_TOPIC
            esp_mqtt_client_publish(mqtt_handle, MQTT_PUB_TOPIC, "Hello MQTT", 0, 0, 0);
            break;

        // 成功取消訂閱 topic
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        // 成功發佈訊息
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        // 收到來自 Broker 的訊息
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            // 印出收到的 topic
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            // 印出收到的資料內容
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        // 發生錯誤事件
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;

        // 處理其他未特別列出的事件
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

void mqtt_init(void){
    esp_mqtt_client_config_t mqtt_cfg = {0};
    mqtt_cfg.broker.address.uri = MQTT_ADDRESS;
    mqtt_cfg.broker.address.port = 1883;
    mqtt_cfg.credentials.username = MQTT_USER;
    mqtt_cfg.credentials.authentication.password = MQTT_PASSWORD;
    mqtt_cfg.credentials.client_id = MQTT_CLIENT_ID;
    mqtt_handle=esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(mqtt_handle, ESP_EVENT_ANY_ID, mqtt_eveny_callback, NULL);

    esp_mqtt_client_start(mqtt_handle);
}