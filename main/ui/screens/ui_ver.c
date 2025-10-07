#include "../ui.h"
#include "../../../components/config_manager/include/config_manager.h"
#include "../../../components/wifi/include/wifi.h"


lv_obj_t *ver_info = NULL;
lv_obj_t *title = NULL;
lv_obj_t *label_ver = NULL;
lv_obj_t *label_mac = NULL;
lv_obj_t *label_wifi = NULL;
lv_obj_t *label_ip = NULL;
lv_obj_t *label_mqtt = NULL;
lv_obj_t *label_back = NULL;
lv_obj_t *label_ipmode = NULL;

static lv_timer_t *ver_update_timer = NULL;

// 宣告更新函式（記得你要實作它）
void ui_ver_update_info(void)
{
    if (label_ver)
        lv_label_set_text_fmt(label_ver, "Version: %s", g_device_config.fw_version);

    if (label_mac)
        lv_label_set_text_fmt(label_mac, "MAC ID: %s", g_device_config.device_mac);

    if (label_wifi) {
        bool wifi_connected = wifi_is_connected();
        const char *status_symbol = wifi_connected ? LV_SYMBOL_OK : LV_SYMBOL_CLOSE;
        const char *active_ssid = wifi_get_connected_ssid();
        const char *display_ssid = (wifi_connected && active_ssid && active_ssid[0] != '\0')
                                       ? active_ssid
                                       : g_device_config.wifi_ssid;
        lv_label_set_text_fmt(label_wifi, "WiFi: %s %s", display_ssid, status_symbol);
        lv_obj_set_style_text_color(label_wifi,
                                    wifi_connected ? lv_color_hex(0x4CAF50) : lv_color_hex(0xF44336),
                                    0);
    }
    if (label_ip) {
        // 根據 ip_mode 決定要顯示哪個 IP
        if (g_device_config.ip_mode == 0)
            lv_label_set_text_fmt(label_ip, "IP: %s", g_device_config.current_ip);
        else if (g_device_config.ip_mode == 1)
            lv_label_set_text_fmt(label_ip, "IP: %s", g_device_config.static_ip);
        else
            lv_label_set_text_fmt(label_ip, "IP: %s", g_device_config.current_ip);
    }

    if (label_mqtt)
        lv_label_set_text_fmt(label_mqtt, "MQTT: %s", g_device_config.mqtt_host);

    if (label_ipmode) {
        // 根據 ip_mode 顯示文字
        if (g_device_config.ip_mode == 0)
            lv_label_set_text(label_ipmode, "IP Mode: DHCP");
        else if (g_device_config.ip_mode == 1)
            lv_label_set_text(label_ipmode, "IP Mode: Static IP");
        else
            lv_label_set_text(label_ipmode, "IP Mode: default");
    }
}

// Timer callback
void ui_ver_update_timer_cb(lv_timer_t *timer)
{
    ui_ver_update_info();   // 每次呼叫更新顯示資訊
}

// 啟動自動更新計時器
void ui_ver_start_auto_update(void)
{
    if (ver_update_timer == NULL) {
        ver_update_timer = lv_timer_create(ui_ver_update_timer_cb, 1000, NULL);
    }
}

void ui_ver_screen_init(void)
{
    ver_info = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ver_info, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(ver_info, LV_OPA_COVER, 0);
    lv_obj_set_size(ver_info, LV_HOR_RES, LV_VER_RES);

    // ---------- 標題 ----------
    title = lv_label_create(ver_info);
    lv_label_set_text(title, "Device Info");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // ---------- FW 版本 ----------
    label_ver = lv_label_create(ver_info);
    lv_label_set_text_fmt(label_ver, "Version: %s", g_device_config.fw_version);
    lv_obj_set_style_text_color(label_ver, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_ver, &lv_font_montserrat_24, 0);
    lv_obj_align(label_ver, LV_ALIGN_TOP_LEFT, 30, 60);

    // ---------- MAC ----------
    label_mac = lv_label_create(ver_info);
    lv_label_set_text_fmt(label_mac, "MAC ID: %s", g_device_config.device_mac);
    lv_obj_set_style_text_color(label_mac, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_mac, &lv_font_montserrat_24, 0);
    lv_obj_align_to(label_mac, label_ver, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    // ---------- WiFi ----------
    label_wifi = lv_label_create(ver_info);
    lv_label_set_text_fmt(label_wifi, "WiFi: %s", g_device_config.wifi_ssid);
    lv_obj_set_style_text_color(label_wifi, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_wifi, &lv_font_montserrat_24, 0);
    lv_obj_align_to(label_wifi, label_mac, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    // ---------- IP ----------
    label_ip = lv_label_create(ver_info);
    if(g_device_config.ip_mode == 0)
        lv_label_set_text_fmt(label_ip, "IP: %s", g_device_config.current_ip);
    else if(g_device_config.ip_mode == 1)
        lv_label_set_text_fmt(label_ip, "IP: %s", g_device_config.static_ip);
    else
        lv_label_set_text_fmt(label_ip, "IP: %s", g_device_config.current_ip);
    lv_obj_set_style_text_color(label_ip, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_ip, &lv_font_montserrat_24, 0);
    lv_obj_align_to(label_ip, label_wifi, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    // ---------- IP Mode ----------
    label_ipmode = lv_label_create(ver_info);
    if(g_device_config.ip_mode == 0)
        lv_label_set_text(label_ipmode, "IP Mode: DHCP");
    else if(g_device_config.ip_mode == 1)
        lv_label_set_text(label_ipmode, "IP Mode: Static IP");
    else
        lv_label_set_text(label_ipmode, "IP Mode: default");
    lv_obj_set_style_text_color(label_ipmode, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_ipmode, &lv_font_montserrat_24, 0);
    lv_obj_align_to(label_ipmode, label_ip, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    // ---------- 返回按鈕 ----------
    label_back = lv_img_create(ver_info);
    lv_img_set_src(label_back, &back);
    lv_obj_set_align(label_back, LV_ALIGN_TOP_LEFT);
    lv_obj_add_flag(label_back, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(label_back, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(label_back, back_btn_event, LV_EVENT_CLICKED, NULL);

    ui_ver_update_info();       // 初始化顯示資訊（含 WiFi 狀態）
    ui_ver_start_auto_update(); // 啟動自動更新
}
