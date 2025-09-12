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
    lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
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
    lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
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
    lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
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
    lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
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
    lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
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
    lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_scene6, ui_event_button1_button, LV_EVENT_ALL, NULL);

    ui_switch_group = lv_obj_create(ui_Home); // 創建下方開關群組
    lv_obj_set_width(ui_switch_group, lv_pct(90)); // 設置寬度為90%
    lv_obj_set_height(ui_switch_group, lv_pct(20)); // 設置高度為20%
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

    ui_sw1 = lv_switch_create(ui_switch_group);
    lv_obj_set_width(ui_sw1, 80);
    lv_obj_set_height(ui_sw1, 40);
    lv_obj_set_align(ui_sw1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_sw1, LV_OBJ_FLAG_CLICKABLE); // 可點擊
    lv_obj_clear_flag(ui_sw1, LV_OBJ_FLAG_SCROLLABLE); // 取消可捲動
    lv_obj_set_style_radius(ui_sw1, 20, LV_PART_MAIN | LV_STATE_DEFAULT); // 圓角
    lv_obj_set_style_bg_color(ui_sw1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 背景白色
    lv_obj_set_style_bg_opa(ui_sw1, 100, LV_PART_MAIN | LV_STATE_DEFAULT); // 背景透明度
    lv_obj_set_style_border_color(ui_sw1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 邊框白色
    lv_obj_set_style_border_width(ui_sw1, 2, LV_PART_MAIN | LV_STATE_DEFAULT); // 邊框寬度
    lv_obj_set_style_border_opa(ui_sw1, 100, LV_PART_MAIN | LV_STATE_DEFAULT); // 邊框透明度
    lv_obj_set_style_pad_all(ui_sw1, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // 內邊距
    lv_obj_set_style_pad_gap(ui_sw1, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // 間距
    lv_obj_set_style_bg_color(ui_sw1, lv_color_hex(0x00FF00), LV_PART_INDICATOR | LV_STATE_CHECKED); // 開啟時背景綠色
    lv_obj_set_style_bg_opa(ui_sw1, 255, LV_PART_INDICATOR | LV_STATE_CHECKED); // 開啟時背景不透明
    lv_obj_set_style_bg_color(ui_sw1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT); // 關閉時背景紅色
    lv_obj_set_style_bg_opa(ui_sw1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 關閉時背景不透明
    lv_obj_set_style_border_color(ui_sw1, lv_color_hex(0x00FF00), LV_PART_INDICATOR | LV_STATE_CHECKED); // 開啟時邊框綠色
    lv_obj_set_style_border_opa(ui_sw1, 255, LV_PART_INDICATOR | LV_STATE_CHECKED); // 開啟時邊框不透明
    lv_obj_set_style_border_color(ui_sw1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT); // 關閉時邊框紅色
    lv_obj_set_style_border_opa(ui_sw1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 關閉時邊框不透明
    lv_obj_set_style_pad_all(ui_sw1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器內邊距
    lv_obj_set_style_pad_gap(ui_sw1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器間距
    lv_obj_set_style_radius(ui_sw1, 20, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器圓角
    lv_obj_set_style_shadow_width(ui_sw1, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 陰影寬度
    lv_obj_set_style_shadow_width(ui_sw1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器陰影寬度
    lv_obj_set_style_shadow_color(ui_sw1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT); // 陰影顏色
    lv_obj_set_style_shadow_color(ui_sw1, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器陰影顏色
    lv_obj_set_style_shadow_opa(ui_sw1, 50, LV_PART_MAIN | LV_STATE_DEFAULT); // 陰影透明度
    lv_obj_set_style_shadow_opa(ui_sw1, 50, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器陰影透明度
    lv_obj_set_style_shadow_spread(ui_sw1, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 陰影擴散
    lv_obj_set_style_shadow_spread(ui_sw1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器陰影擴散
    
    

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


}
