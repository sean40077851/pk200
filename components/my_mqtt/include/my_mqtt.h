#ifndef MY_MQTT_H
#define MY_MQTT_H

#include "mqtt_client.h"   // 提供 esp_mqtt_client_handle_t 和 esp_mqtt_client_publish
#include "esp_event.h"     // 提供 esp_event_base_t

#ifdef __cplusplus
extern "C" {
#endif

extern esp_mqtt_client_handle_t mqtt_handle;

void mqtt_eveny_callback(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_init(void);

#ifdef __cplusplus
}
#endif

#endif // MY_MQTT_H