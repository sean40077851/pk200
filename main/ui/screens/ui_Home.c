#include "../ui.h"  // 包含UI頭文件


lv_obj_t * ui_Home;
lv_obj_t * ui_topbar;       // 上方資訊列
lv_obj_t * ui_logo;         // 左上角 Logo Label/Image
lv_obj_t * ui_date;     // 中上時間日期
lv_obj_t * ui_time;     // 中上時間日期
lv_obj_t * ui_menu_icon;    // 右上角三條線 icon

lv_obj_t * ui_scene_group;  // 場景按鈕群組 (Grid 或 Flex)
lv_obj_t * ui_scene1;
lv_obj_t * ui_scene2;
lv_obj_t * ui_scene3;
lv_obj_t * ui_scene4;
lv_obj_t * ui_scene5;
lv_obj_t * ui_scene6;
lv_obj_t * scene_label;
lv_obj_t * scene_label2;
lv_obj_t * scene_label3;
lv_obj_t * scene_label4;
lv_obj_t * scene_label5;
lv_obj_t * scene_label6;
lv_obj_t * ui_switch_group; // 下方開關群組
lv_obj_t * ui_sw1;
lv_obj_t * ui_sw2;
lv_obj_t * ui_sw3;
lv_obj_t * ui_sw4;
lv_obj_t * ui_sw5;
lv_obj_t * ui_sw6;
lv_obj_t * ui_sw1_icon;
lv_obj_t * ui_sw1_label;
lv_obj_t * ui_sw2_icon;
lv_obj_t * ui_sw2_label;
lv_obj_t * ui_sw3_icon;
lv_obj_t * ui_sw3_label;
lv_obj_t * ui_sw4_icon;
lv_obj_t * ui_sw4_label;
lv_obj_t * ui_sw5_icon;
lv_obj_t * ui_sw5_label;
lv_obj_t * ui_sw6_icon;
lv_obj_t * ui_sw6_label;


void ui_event_menu_icon(lv_event_t * e)      // 菜單圖標事件處理函數
{
    lv_event_code_t event_code = lv_event_get_code(e);  // 獲取事件代碼

    if(event_code == LV_EVENT_CLICKED) {                // 如果是點擊事件
       _ui_screen_change(&ui_Step1, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, &ui_Step1_screen_init);  // 切換到Step1畫面並淡入效果
    }
}
void ui_event_button1_button(lv_event_t * e)           // 按鈕1事件處理函數
{
    lv_event_code_t event_code = lv_event_get_code(e);  // 獲取事件代碼

    if(event_code == LV_EVENT_CLICKED) {                // 如果是點擊事件
        _ui_screen_change(&ui_Step1, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, &ui_Step1_screen_init);  // 切換到Step1畫面並淡入效果
    }
}
void ui_event_sw1(lv_event_t * e) {
    static bool sw1_state = false;
    sw1_state = !sw1_state;

    lv_obj_t * btn = lv_event_get_target(e);

    if(sw1_state) {
        lv_img_set_src(ui_sw1_icon, &OnLight);   // 亮燈圖
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白框
        lv_obj_set_style_border_width(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT); // 粗邊框
    } else {
        lv_img_set_src(ui_sw1_icon, &OffLight);  // 關燈圖
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白框
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT); // 細邊框
    }
}
void ui_event_sw2(lv_event_t * e) {
    static bool sw2_state = false;
    sw2_state = !sw2_state;

    lv_obj_t * btn = lv_event_get_target(e);

    if(sw2_state) {
        lv_img_set_src(ui_sw2_icon, &OnLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_img_set_src(ui_sw2_icon, &OffLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

// SW3
void ui_event_sw3(lv_event_t * e) {
    static bool sw3_state = false;
    sw3_state = !sw3_state;

    lv_obj_t * btn = lv_event_get_target(e);

    if(sw3_state) {
        lv_img_set_src(ui_sw3_icon, &OnLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_img_set_src(ui_sw3_icon, &OffLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

// SW4
void ui_event_sw4(lv_event_t * e) {
    static bool sw4_state = false;
    sw4_state = !sw4_state;

    lv_obj_t * btn = lv_event_get_target(e);

    if(sw4_state) {
        lv_img_set_src(ui_sw4_icon, &OnLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_img_set_src(ui_sw4_icon, &OffLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

// SW5
void ui_event_sw5(lv_event_t * e) {
    static bool sw5_state = false;
    sw5_state = !sw5_state;

    lv_obj_t * btn = lv_event_get_target(e);

    if(sw5_state) {
        lv_img_set_src(ui_sw5_icon, &OnLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_img_set_src(ui_sw5_icon, &OffLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

// SW6
void ui_event_sw6(lv_event_t * e) {
    static bool sw6_state = false;
    sw6_state = !sw6_state;

    lv_obj_t * btn = lv_event_get_target(e);

    if(sw6_state) {
        lv_img_set_src(ui_sw6_icon, &OnLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_img_set_src(ui_sw6_icon, &OffLight);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}
void ui_Home_screen_init(void)          // 主畫面初始化函數
{

     ui_Home = lv_obj_create(NULL);     // 創建主畫面物件，父物件為NULL
    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    // lv_obj_set_style_bg_img_src(ui_Home, &ui_img_bg1_png, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景圖片（註解掉）
    lv_obj_set_style_bg_color(ui_Home, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景顏色為黑色
    lv_obj_set_style_bg_opa(ui_Home, 255, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景不透明


    ui_topbar = lv_obj_create(ui_Home); // 創建頂部資訊列
    lv_obj_set_width(ui_topbar, lv_pct(100)); // 設置寬度為100%
    lv_obj_set_height(ui_topbar, 50); // 設置高度為50像素
    lv_obj_set_x(ui_topbar, 0); // X座標0
    lv_obj_set_y(ui_topbar, 0); // Y座標0
    lv_obj_set_align(ui_topbar, LV_ALIGN_TOP_MID); // 對齊到頂部中間
    lv_obj_clear_flag(ui_topbar, LV_OBJ_FLAG_SCROLLABLE); // 清除可捲動旗標
    lv_obj_set_style_bg_color(ui_topbar, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景顏色為白色
    lv_obj_set_style_bg_opa(ui_topbar, 200, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景透明度

    ui_logo = lv_img_create(ui_topbar); // 創建Logo標籤
    
    lv_img_set_src(ui_logo, &logohome);  // 設定圖片來源為聯絡圖
    lv_obj_set_width(ui_logo, LV_SIZE_CONTENT);    // 寬度自適應
    lv_obj_set_height(ui_logo, LV_SIZE_CONTENT);   // 高度自適應
    lv_obj_set_x(ui_logo, 0);                      // X座標0
    lv_obj_set_y(ui_logo, 0);                    // Y座標0
    lv_obj_set_align(ui_logo, LV_ALIGN_TOP_LEFT);    // 置中對齊
    lv_obj_add_flag(ui_logo, LV_OBJ_FLAG_ADV_HITTEST); // 進階點擊測試
    lv_obj_clear_flag(ui_logo, LV_OBJ_FLAG_SCROLLABLE); // 取消可捲動

    // 日期
    ui_date = lv_label_create(ui_topbar);
    lv_obj_set_width(ui_date, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_date, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_date, -10); // 根據需求微調
    lv_obj_set_y(ui_date, 10);
    lv_obj_set_align(ui_date, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_date, "08/24"); // 兩行顯示
    lv_obj_set_style_text_color(ui_date, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_date, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_date, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_time = lv_label_create(ui_topbar);
    lv_obj_set_width(ui_time, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_time, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_time, 170); // 根據需求微調
    lv_obj_set_y(ui_time, 10);
    lv_obj_set_align(ui_time, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_time, "18:08");
    lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_time, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);


    ui_scene_group = lv_obj_create(ui_Home); // 創建場景按鈕群組
    lv_obj_set_width(ui_scene_group, lv_pct(90)); // 設置寬度為90%
    lv_obj_set_height(ui_scene_group, lv_pct(65)); // 設置高度為50%
    lv_obj_set_x(ui_scene_group, 0); // X座標0
    lv_obj_set_y(ui_scene_group, 0); // Y座標-20
    lv_obj_set_align(ui_scene_group, LV_ALIGN_CENTER); // 對齊到中間
    lv_obj_clear_flag(ui_scene_group, LV_OBJ_FLAG_SCROLLABLE); // 清除可捲動旗標
    lv_obj_set_flex_flow(ui_scene_group, LV_FLEX_FLOW_ROW_WRAP); // 設置為行換行佈局
    lv_obj_set_style_pad_row(ui_scene_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT); // 行間距10
    lv_obj_set_style_pad_column(ui_scene_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT); // 列間距10
    lv_obj_set_style_pad_all(ui_scene_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT); // 內邊距10
    lv_obj_set_style_bg_color(ui_scene_group, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景顏色
    lv_obj_set_style_bg_opa(ui_scene_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景透明度

    ui_scene1 = lv_btn_create(ui_scene_group);
    lv_obj_set_width(ui_scene1, 200);
    lv_obj_set_height(ui_scene1, 50);
    lv_obj_set_style_radius(ui_scene1, 20, LV_PART_MAIN | LV_STATE_DEFAULT); // 圓角
    lv_obj_set_style_border_color(ui_scene1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_border_width(ui_scene1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scene1, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 透明背景
    lv_obj_set_align(ui_scene1, LV_ALIGN_TOP_LEFT);

    scene_label = lv_label_create(ui_scene1);
    lv_label_set_text(scene_label, "SCENE1");
    lv_obj_center(scene_label);
    lv_obj_set_style_text_color(scene_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_scene1, ui_event_button1_button, LV_EVENT_ALL, NULL);
    
    ui_scene2 = lv_btn_create(ui_scene_group);
    lv_obj_set_width(ui_scene2, 200);
    lv_obj_set_height(ui_scene2, 50);
    lv_obj_set_style_radius(ui_scene2, 20, LV_PART_MAIN | LV_STATE_DEFAULT); // 圓角
    lv_obj_set_style_border_color(ui_scene2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_border_width(ui_scene2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scene2, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 透明背景
    lv_obj_set_align(ui_scene2, LV_ALIGN_TOP_RIGHT);

    scene_label2 = lv_label_create(ui_scene2);
    lv_label_set_text(scene_label2, "SCENE2");
    lv_obj_center(scene_label2);
    lv_obj_set_style_text_color(scene_label2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_scene2, ui_event_button1_button, LV_EVENT_ALL, NULL);

    ui_scene3 = lv_btn_create(ui_scene_group);
    lv_obj_set_width(ui_scene3, 200);
    lv_obj_set_height(ui_scene3, 50);
    lv_obj_set_style_radius(ui_scene3, 20, LV_PART_MAIN | LV_STATE_DEFAULT); // 圓角
    lv_obj_set_style_border_color(ui_scene3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_border_width(ui_scene3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scene3, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 透明背景
    lv_obj_set_align(ui_scene3, LV_ALIGN_BOTTOM_LEFT);

    scene_label3 = lv_label_create(ui_scene3);
    lv_label_set_text(scene_label3, "SCENE3");
    lv_obj_center(scene_label3);
    lv_obj_set_style_text_color(scene_label3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_scene3, ui_event_button1_button, LV_EVENT_ALL, NULL);

    ui_scene4 = lv_btn_create(ui_scene_group);
    lv_obj_set_width(ui_scene4, 200);
    lv_obj_set_height(ui_scene4, 50);
    lv_obj_set_style_radius(ui_scene4, 20, LV_PART_MAIN | LV_STATE_DEFAULT); // 圓角
    lv_obj_set_style_border_color(ui_scene4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_border_width(ui_scene4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scene4, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 透明背景
    lv_obj_set_align(ui_scene4, LV_ALIGN_BOTTOM_RIGHT);

    scene_label4 = lv_label_create(ui_scene4);
    lv_label_set_text(scene_label4, "SCENE4");
    lv_obj_center(scene_label4);
    lv_obj_set_style_text_color(scene_label4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_scene4, ui_event_button1_button, LV_EVENT_ALL, NULL);

    ui_scene5 = lv_btn_create(ui_scene_group);
    lv_obj_set_width(ui_scene5, 200);
    lv_obj_set_height(ui_scene5, 50);
    lv_obj_set_style_radius(ui_scene5, 20, LV_PART_MAIN | LV_STATE_DEFAULT); // 圓角
    lv_obj_set_style_border_color(ui_scene5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_border_width(ui_scene5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scene5, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 透明背景
    lv_obj_set_align(ui_scene5, LV_ALIGN_OUT_BOTTOM_LEFT);

    scene_label5 = lv_label_create(ui_scene5);
    lv_label_set_text(scene_label5, "SCENE5");
    lv_obj_center(scene_label5);
    lv_obj_set_style_text_color(scene_label5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_scene5, ui_event_button1_button, LV_EVENT_ALL, NULL);

    ui_scene6 = lv_btn_create(ui_scene_group);
    lv_obj_set_width(ui_scene6, 200);
    lv_obj_set_height(ui_scene6, 50);
    lv_obj_set_style_radius(ui_scene6, 20, LV_PART_MAIN | LV_STATE_DEFAULT); // 圓角
    lv_obj_set_style_border_color(ui_scene6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_border_width(ui_scene6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scene6, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 透明背景
    lv_obj_set_align(ui_scene6, LV_ALIGN_OUT_BOTTOM_RIGHT);

    scene_label6 = lv_label_create(ui_scene6);
    lv_label_set_text(scene_label6, "SCENE6");
    lv_obj_center(scene_label6);
    lv_obj_set_style_text_color(scene_label6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_scene6, ui_event_button1_button, LV_EVENT_ALL, NULL);

    ui_switch_group = lv_obj_create(ui_Home); // 創建下方開關群組
    lv_obj_set_width(ui_switch_group, lv_pct(90)); // 設置寬度為90%
    lv_obj_set_height(ui_switch_group, lv_pct(25)); // 設置高度為20%
    lv_obj_set_x(ui_switch_group, 0); // X座標0
    lv_obj_set_y(ui_switch_group, 0); // Y座標20
    lv_obj_set_align(ui_switch_group, LV_ALIGN_BOTTOM_MID); // 對齊到底部中間
    lv_obj_clear_flag(ui_switch_group, LV_OBJ_FLAG_SCROLLABLE); // 清除可捲動旗標
    lv_obj_set_flex_flow(ui_switch_group, LV_FLEX_FLOW_ROW_WRAP); // 設置為行換行佈局
    lv_obj_set_style_pad_row(ui_switch_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT); // 行間距10
    lv_obj_set_style_pad_column(ui_switch_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT); // 列間距10
    lv_obj_set_style_pad_all(ui_switch_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT); // 內邊距10
    lv_obj_set_style_bg_color(ui_switch_group, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景顏色
    lv_obj_set_style_bg_opa(ui_switch_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置背景透明度 

    ui_sw1 = lv_btn_create(ui_switch_group);
    lv_obj_set_size(ui_sw1, 60,60);
    lv_obj_set_style_radius(ui_sw1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_sw1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_sw1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_sw1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);  // 預設黑底
    lv_obj_set_style_bg_opa(ui_sw1, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 圖示 (燈泡) */
    ui_sw1_icon = lv_img_create(ui_sw1);
    lv_img_set_src(ui_sw1_icon, &OffLight);   // 換成你的燈泡圖資
    lv_obj_align(ui_sw1_icon, LV_ALIGN_TOP_LEFT, 5, 5);

    /* 文字 (SW1) */
    ui_sw1_label = lv_label_create(ui_sw1);
    lv_label_set_text(ui_sw1_label, "SW1");
    lv_obj_set_style_text_color(ui_sw1_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_sw1_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_sw1_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(ui_sw1, ui_event_sw1, LV_EVENT_CLICKED, NULL);


    ui_sw2 = lv_btn_create(ui_switch_group);
    lv_obj_set_size(ui_sw2, 60, 60);
    lv_obj_set_style_radius(ui_sw2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_sw2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_sw2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_sw2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sw2, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sw2_icon = lv_img_create(ui_sw2);
    lv_img_set_src(ui_sw2_icon, &OffLight); // 換成你的燈泡圖資
    lv_obj_align(ui_sw2_icon, LV_ALIGN_TOP_LEFT, 5, 5);

    ui_sw2_label = lv_label_create(ui_sw2);
    lv_label_set_text(ui_sw2_label, "SW2");
    lv_obj_set_style_text_color(ui_sw2_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_sw2_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_sw2_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(ui_sw2, ui_event_sw2, LV_EVENT_CLICKED, NULL);

    // SW3
    ui_sw3 = lv_btn_create(ui_switch_group);
    lv_obj_set_size(ui_sw3, 60, 60);
    lv_obj_set_style_radius(ui_sw3, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_sw3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_sw3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_sw3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sw3, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sw3_icon = lv_img_create(ui_sw3);
    lv_img_set_src(ui_sw3_icon, &OffLight);
    lv_obj_align(ui_sw3_icon, LV_ALIGN_TOP_LEFT, 5, 5);

    ui_sw3_label = lv_label_create(ui_sw3);
    lv_label_set_text(ui_sw3_label, "SW3");
    lv_obj_set_style_text_color(ui_sw3_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_sw3_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_sw3_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(ui_sw3, ui_event_sw3, LV_EVENT_CLICKED, NULL);

    // SW4
    ui_sw4 = lv_btn_create(ui_switch_group);
    lv_obj_set_size(ui_sw4, 60, 60);
    lv_obj_set_style_radius(ui_sw4, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_sw4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_sw4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_sw4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sw4, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sw4_icon = lv_img_create(ui_sw4);
    lv_img_set_src(ui_sw4_icon, &OffLight);
    lv_obj_align(ui_sw4_icon, LV_ALIGN_TOP_LEFT, 5, 5);

    ui_sw4_label = lv_label_create(ui_sw4);
    lv_label_set_text(ui_sw4_label, "SW4");
    lv_obj_set_style_text_color(ui_sw4_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_sw4_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_sw4_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(ui_sw4, ui_event_sw4, LV_EVENT_CLICKED, NULL);

    // SW5
    ui_sw5 = lv_btn_create(ui_switch_group);
    lv_obj_set_size(ui_sw5, 60, 60);
    lv_obj_set_style_radius(ui_sw5, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_sw5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_sw5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_sw5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sw5, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sw5_icon = lv_img_create(ui_sw5);
    lv_img_set_src(ui_sw5_icon, &OffLight);
    lv_obj_align(ui_sw5_icon, LV_ALIGN_TOP_LEFT, 5, 5);

    ui_sw5_label = lv_label_create(ui_sw5);
    lv_label_set_text(ui_sw5_label, "SW5");
    lv_obj_set_style_text_color(ui_sw5_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_sw5_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_sw5_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(ui_sw5, ui_event_sw5, LV_EVENT_CLICKED, NULL);

    // SW6
    ui_sw6 = lv_btn_create(ui_switch_group);
    lv_obj_set_size(ui_sw6, 60, 60);
    lv_obj_set_style_radius(ui_sw6, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_sw6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_sw6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_sw6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sw6, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sw6_icon = lv_img_create(ui_sw6);
    lv_img_set_src(ui_sw6_icon, &OffLight);
    lv_obj_align(ui_sw6_icon, LV_ALIGN_TOP_LEFT, 5, 5);

    ui_sw6_label = lv_label_create(ui_sw6);
    lv_label_set_text(ui_sw6_label, "SW6");
    lv_obj_set_style_text_color(ui_sw6_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_sw6_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_sw6_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(ui_sw6, ui_event_sw6, LV_EVENT_CLICKED, NULL);

}

void ui_Home_screen_destroy(void)
{
    if(ui_Home) lv_obj_del(ui_Home);

    // NULL screen variables
    ui_Home = NULL;
    ui_topbar = NULL;
    ui_logo = NULL;
    ui_date = NULL;
    ui_time = NULL;
    ui_menu_icon = NULL;
    ui_scene_group = NULL;
    ui_scene1 = NULL;
    ui_scene2 = NULL;
    ui_scene3 = NULL;
    ui_scene4 = NULL;
    ui_scene5 = NULL;
    ui_scene6 = NULL;
    scene_label= NULL;
    ui_switch_group = NULL;
    ui_sw1 = NULL;
    ui_sw2 = NULL;
    ui_sw3 = NULL;
    ui_sw4 = NULL;
    ui_sw5 = NULL;
    ui_sw6 = NULL;
    ui_sw1_icon = NULL;
    ui_sw1_label = NULL;


}
