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


// ========== 對外函式 ==========
/**
 * @brief 初始化版本/資訊頁面
 * 內容：FW Ver、MAC、WiFi、IP、MQTT
 */
void ui_ver_screen_init(void);



#endif // UI_VER_H