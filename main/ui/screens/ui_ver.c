#include "../ui.h"  // 包含UI頭文件
#include "../../../components/config_manager/include/config_manager.h" // 引入設定管理相關的標頭檔

lv_obj_t *ver_info=NULL;
lv_obj_t *title=NULL;
lv_obj_t *label_ver=NULL;
lv_obj_t *label_mac=NULL;
lv_obj_t *label_wifi=NULL;
lv_obj_t *label_ip=NULL;
lv_obj_t *label_mqtt=NULL;
lv_obj_t *label_back=NULL;

// 建立第一個頁面
void ui_ver_screen_init(void)
{
    ver_info = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ver_info, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(ver_info, LV_OPA_COVER, 0);
    lv_obj_set_size(ver_info, LV_HOR_RES, LV_VER_RES);

    
    title = lv_label_create(ver_info);
    lv_label_set_text(title, "Device Info");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // ---------- FW 版本 ----------
    label_ver = lv_label_create(ver_info);
    lv_label_set_text(label_ver, "Version:PK200-wentai-v0.6.1");
    lv_obj_set_style_text_color(label_ver, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_ver, LV_ALIGN_TOP_LEFT, 30, 60);  // 左上角 (10,10)
    lv_obj_set_style_text_font(label_ver, &lv_font_montserrat_24, 0);  

    // ---------- MAC ----------
    label_mac = lv_label_create(ver_info);
    lv_label_set_text_fmt(label_mac, "MAC ID: %s", g_device_config.device_mac);
    lv_obj_set_style_text_color(label_mac, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(label_mac, label_ver, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); // 放在版本下面
    lv_obj_set_style_text_font(label_mac, &lv_font_montserrat_24, 0);  

    // ---------- WiFi ----------
    label_wifi = lv_label_create(ver_info);
    lv_label_set_text_fmt(label_wifi, "WiFi: %s", g_device_config.wifi_ssid);
    lv_obj_set_style_text_color(label_wifi, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(label_wifi, label_mac, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_text_font(label_wifi, &lv_font_montserrat_24, 0);  

    // ---------- IP ----------
    label_ip = lv_label_create(ver_info);
    lv_label_set_text_fmt(label_ip, "IP: %s", g_device_config.current_ip);
    lv_obj_set_style_text_color(label_ip, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(label_ip, label_wifi, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_text_font(label_ip, &lv_font_montserrat_24, 0);  

    // ---------- MQTT ----------
    /*label_mqtt = lv_label_create(ver_info);
    lv_label_set_text_fmt(label_mqtt, "MQTT.host: %s", g_device_config.mqtt_host);
    lv_obj_set_style_text_color(label_mqtt, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(label_mqtt, label_ip, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_text_font(label_mqtt, &lv_font_montserrat_24, 0);  
*/



    label_back = lv_img_create(ver_info); // 創建Logo標籤
    lv_img_set_src(label_back, &back);  // 設定圖片來源為聯絡圖
    lv_obj_set_width(label_back, LV_SIZE_CONTENT);    // 寬度自適應
    lv_obj_set_height(label_back, LV_SIZE_CONTENT);   // 高度自適應
    lv_obj_set_align(label_back, LV_ALIGN_TOP_LEFT);    
    lv_obj_add_flag(label_back, LV_OBJ_FLAG_CLICKABLE); // 可點擊
    lv_obj_clear_flag(label_back, LV_OBJ_FLAG_SCROLLABLE); // 取消可捲動
    lv_obj_add_event_cb(label_back, back_btn_event, LV_EVENT_CLICKED, NULL);
    
    

}