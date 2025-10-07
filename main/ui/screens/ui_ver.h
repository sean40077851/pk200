#ifndef UI_VER_H
#define UI_VER_H





extern lv_obj_t *ver_info;
extern lv_obj_t *title;
extern lv_obj_t *label_ver;
extern lv_obj_t *label_mac;
extern lv_obj_t *label_wifi;
extern lv_obj_t *label_ip;
extern lv_obj_t *label_mqtt;
extern lv_obj_t *label_back;
extern lv_obj_t *label_ipmode;

// ========== 對外函式 ==========
/**
 * @brief 初始化版本/資訊頁面
 * 內容：FW Ver、MAC、WiFi、IP、MQTT
 */
void ui_ver_screen_init(void);
void ui_ver_start_auto_update(void);
void ui_ver_update_info(void);
void ui_ver_update_timer_cb(lv_timer_t *timer);


#endif // UI_VER_H