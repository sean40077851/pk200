// 此文件由 SquareLine Studio 生成
// SquareLine Studio 版本: SquareLine Studio 1.5.3
// LVGL 版本: 8.3.6
// 項目名稱: EV_Charger

#include "../ui.h"  // 包含UI頭文件

// 全域UI物件指標宣告
lv_obj_t * ui_Home = NULL;              // 主畫面物件
lv_obj_t * ui_content_group1 = NULL;    // 內容群組1物件
lv_obj_t * ui_slidebar1 = NULL;         // 側邊欄物件
lv_obj_t * ui_icon_group1 = NULL;       // 圖標群組1物件
lv_obj_t * ui_pricing_icon1 = NULL;     // 定價圖標物件
lv_obj_t * ui_pricing_label1 = NULL;    // 定價標籤物件
lv_obj_t * ui_icon_group2 = NULL;       // 圖標群組2物件
lv_obj_t * ui_help_icon1 = NULL;        // 幫助圖標物件
lv_obj_t * ui_help_label1 = NULL;       // 幫助標籤物件
lv_obj_t * ui_content1 = NULL;          // 內容區域1物件
lv_obj_t * ui_info_group1 = NULL;       // 資訊群組1物件
lv_obj_t * ui_contact_icon1 = NULL;     // 聯絡方式圖標物件
lv_obj_t * ui_call_number1 = NULL;      // 電話號碼標籤物件
lv_obj_t * ui_date1 = NULL;             // 日期標籤物件
lv_obj_t * ui_charger_id = NULL;        // 充電器ID標籤物件
lv_obj_t * ui_card = NULL;              // 卡片物件
lv_obj_t * ui_button_group = NULL;      // 按鈕群組物件
lv_obj_t * ui_button1 = NULL;           // 按鈕1物件
lv_obj_t * ui_Image2 = NULL;            // 圖片2物件

// 事件處理函數
void ui_event_Home(lv_event_t * e)      // 主畫面事件處理函數
{
    lv_event_code_t event_code = lv_event_get_code(e);  // 獲取事件代碼

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {      // 如果是畫面開始載入事件
        left_on_Animation(ui_card, 0);                  // 執行卡片從左側進入動畫
        bottom_on_Animation(ui_button1, 0);             // 執行按鈕從下方進入動畫
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {    // 如果是畫面開始卸載事件
        left_off_Animation(ui_card, 0);                 // 執行卡片向左側退出動畫
        bottom_off_Animation(ui_button1, 0);            // 執行按鈕向下方退出動畫
    }
}

void ui_event_button1_button(lv_event_t * e)           // 按鈕1事件處理函數
{
    lv_event_code_t event_code = lv_event_get_code(e);  // 獲取事件代碼

    if(event_code == LV_EVENT_CLICKED) {                // 如果是點擊事件
        _ui_screen_change(&ui_Step1, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, &ui_Step1_screen_init);  // 切換到Step1畫面並淡入效果
    }
}

// 建構函數

void ui_Home_screen_init(void)          // 主畫面初始化函數
{
    ui_Home = lv_obj_create(NULL);      // 創建主畫面物件，父物件為NULL
    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    lv_obj_set_style_bg_img_src(ui_Home, &ui_img_bg1_png, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景圖片

    ui_content_group1 = lv_obj_create(ui_Home);             // 在主畫面上創建內容群組
    lv_obj_set_width(ui_content_group1, lv_pct(100));       // 設置寬度為100%
    lv_obj_set_height(ui_content_group1, lv_pct(100));      // 設置高度為100%
    lv_obj_set_align(ui_content_group1, LV_ALIGN_CENTER);   // 設置對齊方式為居中
    lv_obj_set_flex_flow(ui_content_group1, LV_FLEX_FLOW_ROW);  // 設置彈性佈局為橫向排列
    lv_obj_set_flex_align(ui_content_group1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);  // 設置彈性對齊方式
    lv_obj_clear_flag(ui_content_group1, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    lv_obj_set_style_bg_color(ui_content_group1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景顏色為白色
    lv_obj_set_style_bg_opa(ui_content_group1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景透明度為0（完全透明）

    ui_slidebar1 = lv_obj_create(ui_content_group1);        // 在內容群組中創建側邊欄
    lv_obj_set_width(ui_slidebar1, 50);                     // 設置側邊欄寬度為50像素
    lv_obj_set_height(ui_slidebar1, lv_pct(100));           // 設置側邊欄高度為100%
    lv_obj_set_x(ui_slidebar1, 0);                          // 設置X座標為0
    lv_obj_set_y(ui_slidebar1, -33);                        // 設置Y座標為-33（向上偏移33像素）
    lv_obj_set_align(ui_slidebar1, LV_ALIGN_LEFT_MID);      // 設置對齊方式為左側中間
    lv_obj_clear_flag(ui_slidebar1, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    lv_obj_set_style_bg_img_src(ui_slidebar1, &ui_img_left_side_png, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置側邊欄背景圖片
    lv_obj_set_style_pad_left(ui_slidebar1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);    // 設置左邊距為0
    lv_obj_set_style_pad_right(ui_slidebar1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);   // 設置右邊距為0
    lv_obj_set_style_pad_top(ui_slidebar1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);     // 設置上邊距為0
    lv_obj_set_style_pad_bottom(ui_slidebar1, 40, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置下邊距為40像素
    lv_obj_set_style_pad_row(ui_slidebar1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);    // 設置行間距為10像素
    lv_obj_set_style_pad_column(ui_slidebar1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置列間距為0

    ui_icon_group1 = lv_obj_create(ui_slidebar1);           // 在側邊欄中創建圖標群組1
    lv_obj_set_width(ui_icon_group1, 38);                   // 設置寬度為38像素
    lv_obj_set_height(ui_icon_group1, 38);                  // 設置高度為38像素
    lv_obj_set_x(ui_icon_group1, 0);                        // 設置X座標為0
    lv_obj_set_y(ui_icon_group1, 89);                       // 設置Y座標為89
    lv_obj_set_align(ui_icon_group1, LV_ALIGN_CENTER);      // 設置對齊方式為居中
    lv_obj_set_flex_flow(ui_icon_group1, LV_FLEX_FLOW_COLUMN);  // 設置彈性佈局為縱向排列
    lv_obj_set_flex_align(ui_icon_group1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);  // 設置彈性對齊為居中
    lv_obj_clear_flag(ui_icon_group1, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    lv_obj_set_style_bg_color(ui_icon_group1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景顏色為白色
    lv_obj_set_style_bg_opa(ui_icon_group1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景透明度為0
    lv_obj_set_style_pad_row(ui_icon_group1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);    // 設置行間距為8像素
    lv_obj_set_style_pad_column(ui_icon_group1, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置列間距為0
    lv_obj_set_style_radius(ui_icon_group1, 12, LV_PART_MAIN | LV_STATE_PRESSED);    // 設置按下狀態的圓角為12像素
    lv_obj_set_style_bg_color(ui_icon_group1, lv_color_hex(0x22223C), LV_PART_MAIN | LV_STATE_PRESSED);  // 設置按下狀態的背景顏色
    lv_obj_set_style_bg_opa(ui_icon_group1, 255, LV_PART_MAIN | LV_STATE_PRESSED);   // 設置按下狀態的透明度為255（不透明）

    ui_pricing_icon1 = lv_img_create(ui_icon_group1);       // 在圖標群組1中創建定價圖標
    lv_img_set_src(ui_pricing_icon1, &ui_img_pricing_png);  // 設置圖標來源
    lv_obj_set_width(ui_pricing_icon1, LV_SIZE_CONTENT);    // 設置寬度為內容大小
    lv_obj_set_height(ui_pricing_icon1, LV_SIZE_CONTENT);   // 設置高度為內容大小
    lv_obj_set_align(ui_pricing_icon1, LV_ALIGN_CENTER);    // 設置對齊方式為居中
    lv_obj_add_flag(ui_pricing_icon1, LV_OBJ_FLAG_ADV_HITTEST);     // 添加進階點擊測試旗標
    lv_obj_clear_flag(ui_pricing_icon1, LV_OBJ_FLAG_SCROLLABLE);    // 清除可捲動旗標

    ui_pricing_label1 = lv_label_create(ui_icon_group1);    // 在圖標群組1中創建定價標籤
    lv_obj_set_width(ui_pricing_label1, LV_SIZE_CONTENT);   // 設置寬度為內容大小
    lv_obj_set_height(ui_pricing_label1, LV_SIZE_CONTENT);  // 設置高度為內容大小
    lv_obj_set_x(ui_pricing_label1, 8);                     // 設置X座標為8
    lv_obj_set_y(ui_pricing_label1, 19);                    // 設置Y座標為19
    lv_obj_set_align(ui_pricing_label1, LV_ALIGN_CENTER);   // 設置對齊方式為居中
    lv_label_set_text(ui_pricing_label1, "Pricing");        // 設置標籤文字為"Pricing"
    lv_obj_set_style_text_color(ui_pricing_label1, lv_color_hex(0xFEFEFE), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字顏色為白色
    lv_obj_set_style_text_opa(ui_pricing_label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字透明度為255（不透明）
    lv_obj_set_style_text_font(ui_pricing_label1, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置字體

    ui_icon_group2 = lv_obj_create(ui_slidebar1);           // 在側邊欄中創建圖標群組2
    lv_obj_set_width(ui_icon_group2, 38);                   // 設置寬度為38像素
    lv_obj_set_height(ui_icon_group2, 38);                  // 設置高度為38像素
    lv_obj_set_x(ui_icon_group2, 0);                        // 設置X座標為0
    lv_obj_set_y(ui_icon_group2, 149);                      // 設置Y座標為149
    lv_obj_set_align(ui_icon_group2, LV_ALIGN_CENTER);      // 設置對齊方式為居中
    lv_obj_set_flex_flow(ui_icon_group2, LV_FLEX_FLOW_COLUMN);  // 設置彈性佈局為縱向排列
    lv_obj_set_flex_align(ui_icon_group2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);  // 設置彈性對齊為居中
    lv_obj_clear_flag(ui_icon_group2, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    lv_obj_set_style_bg_color(ui_icon_group2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景顏色為白色
    lv_obj_set_style_bg_opa(ui_icon_group2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景透明度為0
    lv_obj_set_style_pad_row(ui_icon_group2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);    // 設置行間距為8像素
    lv_obj_set_style_pad_column(ui_icon_group2, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置列間距為0
    lv_obj_set_style_radius(ui_icon_group2, 12, LV_PART_MAIN | LV_STATE_PRESSED);    // 設置按下狀態的圓角為12像素
    lv_obj_set_style_bg_color(ui_icon_group2, lv_color_hex(0x22223C), LV_PART_MAIN | LV_STATE_PRESSED);  // 設置按下狀態的背景顏色
    lv_obj_set_style_bg_opa(ui_icon_group2, 255, LV_PART_MAIN | LV_STATE_PRESSED);   // 設置按下狀態的透明度為255（不透明）

    ui_help_icon1 = lv_img_create(ui_icon_group2);          // 在圖標群組2中創建幫助圖標
    lv_img_set_src(ui_help_icon1, &ui_img_help_png);        // 設置圖標來源
    lv_obj_set_width(ui_help_icon1, LV_SIZE_CONTENT);       // 設置寬度為內容大小
    lv_obj_set_height(ui_help_icon1, LV_SIZE_CONTENT);      // 設置高度為內容大小
    lv_obj_set_align(ui_help_icon1, LV_ALIGN_CENTER);       // 設置對齊方式為居中
    lv_obj_add_flag(ui_help_icon1, LV_OBJ_FLAG_ADV_HITTEST);     // 添加進階點擊測試旗標
    lv_obj_clear_flag(ui_help_icon1, LV_OBJ_FLAG_SCROLLABLE);    // 清除可捲動旗標

    ui_help_label1 = lv_label_create(ui_icon_group2);       // 在圖標群組2中創建幫助標籤
    lv_obj_set_width(ui_help_label1, LV_SIZE_CONTENT);      // 設置寬度為內容大小
    lv_obj_set_height(ui_help_label1, LV_SIZE_CONTENT);     // 設置高度為內容大小
    lv_obj_set_align(ui_help_label1, LV_ALIGN_CENTER);      // 設置對齊方式為居中
    lv_label_set_text(ui_help_label1, "Help");              // 設置標籤文字為"Help"
    lv_obj_set_style_text_color(ui_help_label1, lv_color_hex(0xFEFEFE), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字顏色為白色
    lv_obj_set_style_text_opa(ui_help_label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字透明度為255（不透明）
    lv_obj_set_style_text_font(ui_help_label1, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置字體

    ui_content1 = lv_obj_create(ui_content_group1);         // 在內容群組1中創建內容區域
    lv_obj_set_height(ui_content1, lv_pct(100));            // 設置高度為100%
    lv_obj_set_flex_grow(ui_content1, 1);                   // 設置彈性增長係數為1
    lv_obj_set_align(ui_content1, LV_ALIGN_CENTER);         // 設置對齊方式為居中
    lv_obj_clear_flag(ui_content1, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    lv_obj_set_style_bg_color(ui_content1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景顏色為白色
    lv_obj_set_style_bg_opa(ui_content1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景透明度為0
    lv_obj_set_style_pad_left(ui_content1, 30, LV_PART_MAIN | LV_STATE_DEFAULT);   // 設置左邊距為30像素
    lv_obj_set_style_pad_right(ui_content1, 30, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置右邊距為30像素
    lv_obj_set_style_pad_top(ui_content1, 30, LV_PART_MAIN | LV_STATE_DEFAULT);    // 設置上邊距為30像素
    lv_obj_set_style_pad_bottom(ui_content1, 30, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置下邊距為30像素

    ui_info_group1 = lv_obj_create(ui_content1);            // 在內容區域中創建資訊群組
    lv_obj_set_width(ui_info_group1, LV_SIZE_CONTENT);      // 設置寬度為內容大小
    lv_obj_set_height(ui_info_group1, LV_SIZE_CONTENT);     // 設置高度為內容大小
    lv_obj_set_x(ui_info_group1, -23);                      // 設置X座標為-23
    lv_obj_set_y(ui_info_group1, -18);                      // 設置Y座標為-18
    lv_obj_set_flex_flow(ui_info_group1, LV_FLEX_FLOW_ROW); // 設置彈性佈局為橫向排列
    lv_obj_set_flex_align(ui_info_group1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);  // 設置彈性對齊為居中
    lv_obj_clear_flag(ui_info_group1, LV_OBJ_FLAG_SCROLLABLE);      // 清除可捲動旗標
    lv_obj_set_style_bg_color(ui_info_group1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景顏色為白色
    lv_obj_set_style_bg_opa(ui_info_group1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置背景透明度為0
    lv_obj_set_style_pad_row(ui_info_group1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);    // 設置行間距為0
    lv_obj_set_style_pad_column(ui_info_group1, 8, LV_PART_MAIN | LV_STATE_DEFAULT); // 設置列間距為8像素

    ui_contact_icon1 = lv_img_create(ui_info_group1);       // 在資訊群組中創建聯絡方式圖標
    lv_img_set_src(ui_contact_icon1, &ui_img_contact_png);  // 設置圖標來源
    lv_obj_set_width(ui_contact_icon1, LV_SIZE_CONTENT);    // 設置寬度為內容大小
    lv_obj_set_height(ui_contact_icon1, LV_SIZE_CONTENT);   // 設置高度為內容大小
    lv_obj_set_align(ui_contact_icon1, LV_ALIGN_CENTER);    // 設置對齊方式為居中
    lv_obj_add_flag(ui_contact_icon1, LV_OBJ_FLAG_ADV_HITTEST);     // 添加進階點擊測試旗標
    lv_obj_clear_flag(ui_contact_icon1, LV_OBJ_FLAG_SCROLLABLE);    // 清除可捲動旗標

    ui_call_number1 = lv_label_create(ui_info_group1);      // 在資訊群組中創建電話號碼標籤
    lv_obj_set_width(ui_call_number1, LV_SIZE_CONTENT);     // 設置寬度為內容大小
    lv_obj_set_height(ui_call_number1, LV_SIZE_CONTENT);    // 設置高度為內容大小
    lv_obj_set_align(ui_call_number1, LV_ALIGN_CENTER);     // 設置對齊方式為居中
    lv_label_set_text(ui_call_number1, "www.fusiontech.com.tw");  // 設置標籤文字為網址
    lv_obj_set_style_text_color(ui_call_number1, lv_color_hex(0x242424), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字顏色為深灰色
    lv_obj_set_style_text_opa(ui_call_number1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字透明度為255（不透明）
    lv_obj_set_style_text_font(ui_call_number1, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置字體

    ui_date1 = lv_label_create(ui_content1);                // 在內容區域中創建日期標籤
    lv_obj_set_width(ui_date1, LV_SIZE_CONTENT);            // 設置寬度為內容大小
    lv_obj_set_height(ui_date1, LV_SIZE_CONTENT);           // 設置高度為內容大小
    lv_obj_set_x(ui_date1, 21);                             // 設置X座標為21
    lv_obj_set_y(ui_date1, -18);                            // 設置Y座標為-18
    lv_obj_set_align(ui_date1, LV_ALIGN_TOP_RIGHT);         // 設置對齊方式為右上角
    lv_label_set_text(ui_date1, "01.03.2023. 14:49 PM");   // 設置標籤文字為日期時間
    lv_obj_set_style_text_color(ui_date1, lv_color_hex(0x242424), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字顏色為深灰色
    lv_obj_set_style_text_opa(ui_date1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字透明度為255（不透明）
    lv_obj_set_style_text_font(ui_date1, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置字體

    ui_charger_id = lv_label_create(ui_content1);           // 在內容區域中創建充電器ID標籤
    lv_obj_set_width(ui_charger_id, LV_SIZE_CONTENT);       // 設置寬度為內容大小
    lv_obj_set_height(ui_charger_id, LV_SIZE_CONTENT);      // 設置高度為內容大小
    lv_obj_set_x(ui_charger_id, -20);                       // 設置X座標為-20
    lv_obj_set_y(ui_charger_id, 18);                        // 設置Y座標為18
    lv_obj_set_align(ui_charger_id, LV_ALIGN_BOTTOM_LEFT);  // 設置對齊方式為左下角
    lv_label_set_text(ui_charger_id, "Charger #5879965-08"); // 設置標籤文字為充電器ID
    lv_obj_set_style_text_color(ui_charger_id, lv_color_hex(0x242424), LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字顏色為深灰色
    lv_obj_set_style_text_opa(ui_charger_id, 255, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置文字透明度為255（不透明）
    lv_obj_set_style_text_font(ui_charger_id, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);  // 設置字體

    ui_card = ui_card_create(ui_Home);                      // 在主畫面上創建自定義卡片物件
    lv_obj_set_x(ui_card, -100);                            // 設置X座標為-100
    lv_obj_set_y(ui_card, -16);                             // 設置Y座標為-16

    
    // 設定卡片內的標題文字為 "Get started!"
    lv_label_set_text(ui_comp_get_child(ui_card, UI_COMP_CARD_CARDBG2_CARD_H), "Get started!");
    
    // 設定卡片內的描述文字
    lv_label_set_text(ui_comp_get_child(ui_card, UI_COMP_CARD_CARDBG2_CARD_TEXT),
                     "The world's fastest and best charger is at your disposal.");
    
    // 設定卡片內的主標題為 "Welcome!"
    lv_label_set_text(ui_comp_get_child(ui_card, UI_COMP_CARD_CARDBG2_CARD_TITLE), "Welcome!");

    // 創建按鈕群組容器
    ui_button_group = lv_obj_create(ui_Home);
    
    // 設定按鈕群組寬度為父容器的100%
    lv_obj_set_width(ui_button_group, lv_pct(100));
    
    // 設定按鈕群組高度為父容器的100%
    lv_obj_set_height(ui_button_group, lv_pct(100));
    
    // 設定按鈕群組對齊方式為置中
    lv_obj_set_align(ui_button_group, LV_ALIGN_CENTER);
    
    // 清除按鈕群組的可點擊和可滾動標誌
    lv_obj_clear_flag(ui_button_group, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    /// 樣式設定標誌
    // 設定按鈕群組背景顏色為白色
    lv_obj_set_style_bg_color(ui_button_group, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 設定按鈕群組背景透明度為0（完全透明）
    lv_obj_set_style_bg_opa(ui_button_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 設定按鈕群組左邊內邊距為30像素
    lv_obj_set_style_pad_left(ui_button_group, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 設定按鈕群組右邊內邊距為30像素
    lv_obj_set_style_pad_right(ui_button_group, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 設定按鈕群組上方內邊距為30像素
    lv_obj_set_style_pad_top(ui_button_group, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 設定按鈕群組下方內邊距為30像素
    lv_obj_set_style_pad_bottom(ui_button_group, 30, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 創建第一個按鈕
    ui_button1 = ui_button_create(ui_button_group);
    
    // 設定按鈕1寬度為150像素
    lv_obj_set_width(ui_button1, 150);
    
    // 設定按鈕1高度為40像素
    lv_obj_set_height(ui_button1, 40);
    
    // 設定按鈕1的X座標偏移為20像素
    lv_obj_set_x(ui_button1, 20);
    
    // 設定按鈕1的Y座標偏移為18像素
    lv_obj_set_y(ui_button1, 18);
    
    // 設定按鈕1對齊方式為右下角
    lv_obj_set_align(ui_button1, LV_ALIGN_BOTTOM_RIGHT);

    // 設定按鈕1的標籤文字為 "Start Charging"
    lv_label_set_text(ui_comp_get_child(ui_button1, UI_COMP_BUTTON_BUTTON_LABEL), "Start Charging");

    // 創建圖片物件
    ui_Image2 = lv_img_create(ui_Home);
    
    // 設定圖片來源為汽車圖片
    lv_img_set_src(ui_Image2, &ui_img_s1_car_png);
    
    // 設定圖片寬度為內容大小（378像素）
    lv_obj_set_width(ui_Image2, LV_SIZE_CONTENT);   /// 378
    
    // 設定圖片高度為內容大小（185像素）
    lv_obj_set_height(ui_Image2, LV_SIZE_CONTENT);    /// 185
    
    // 設定圖片X座標為63像素
    lv_obj_set_x(ui_Image2, 63);
    
    // 設定圖片Y座標為44像素
    lv_obj_set_y(ui_Image2, 44);
    
    // 設定圖片對齊方式為置中
    lv_obj_set_align(ui_Image2, LV_ALIGN_CENTER);
    
    // 添加高級點擊測試標誌
    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_ADV_HITTEST);     /// 樣式標誌
    
    // 清除圖片的可滾動標誌
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);      /// 樣式標誌

    // 為按鈕1添加事件回調函數
    lv_obj_add_event_cb(ui_button1, ui_event_button1_button, LV_EVENT_ALL, NULL);
    
    // 為主頁面添加事件回調函數
    lv_obj_add_event_cb(ui_Home, ui_event_Home, LV_EVENT_ALL, NULL);
}

// 銷毀主頁面畫面的函數
void ui_Home_screen_destroy(void) {
    // 如果主頁面物件存在，則刪除它
    if(ui_Home) lv_obj_del(ui_Home);

    // 將所有畫面變數設為 NULL
    ui_Home = NULL;
    ui_content_group1 = NULL;
    ui_slidebar1 = NULL;
    ui_icon_group1 = NULL;
    ui_pricing_icon1 = NULL;
    ui_pricing_label1 = NULL;
    ui_icon_group2 = NULL;
    ui_help_icon1 = NULL;
    ui_help_label1 = NULL;
    ui_content1 = NULL;
    ui_info_group1 = NULL;
    ui_contact_icon1 = NULL;
    ui_call_number1 = NULL;
    ui_date1 = NULL;
    ui_charger_id = NULL;
    ui_card = NULL;
    ui_button_group = NULL;
    ui_button1 = NULL;
    ui_Image2 = NULL;
}