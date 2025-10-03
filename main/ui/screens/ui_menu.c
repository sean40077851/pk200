#include "../ui.h"  // 包含UI頭文件


lv_obj_t *scr_menu=NULL;
lv_obj_t *time_menu=NULL;
lv_obj_t *time_menu_im=NULL;
lv_obj_t *menu_back=NULL;
lv_obj_t *ver_menu=NULL;
lv_obj_t *ver_menu_im=NULL;
lv_obj_t *menu1=NULL;
lv_obj_t *menu2=NULL;
lv_obj_t *menu3=NULL;


// 按鈕事件回調：切換到第二頁
void btn_event_cb(lv_event_t *e)
{
     lv_event_code_t event_code = lv_event_get_code(e);  // 獲取事件代碼

    if(event_code == LV_EVENT_CLICKED) {                // 如果是點擊事件
       _ui_screen_change(&ui_Step2, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, &ui_Step2_screen_init);  // 切換到Step1畫面並淡入效果
    }
}   
void menu_back_btn_event(lv_event_t * e)
{
    lv_scr_load(ui_Home);  
}
void menu_to_ver_event(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);  // 獲取事件代碼

    if(event_code == LV_EVENT_CLICKED) {                // 如果是點擊事件
       _ui_screen_change(&ver_info, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, &ui_ver_screen_init);  
    }  
}

// 建立第一個頁面
void ui_menu_screen_init(void)
{
    // 建立第一個頁面 (背景黑色，可捲動)
    scr_menu = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr_menu, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scr_menu, LV_OPA_COVER, 0);
    lv_obj_set_scroll_dir(scr_menu, LV_DIR_VER);   // 垂直方向可捲動
    lv_obj_set_size(scr_menu, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_scrollbar_mode(scr_menu, LV_SCROLLBAR_MODE_AUTO);

    

    // 建立第一顆按鈕
    time_menu = lv_btn_create(scr_menu);
    lv_obj_set_size(time_menu, 110, 110);
    lv_obj_align(time_menu, LV_ALIGN_TOP_LEFT, 40, 55);
    lv_obj_set_style_bg_opa(time_menu, LV_OPA_TRANSP, 0);     // 背景透明
    lv_obj_set_style_border_width(time_menu, 2, 0);           // 邊框寬度 2px
    lv_obj_set_style_border_color(time_menu, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_radius(time_menu, 0, 0);                 // 直角，不要圓角
    lv_obj_add_flag(time_menu, LV_OBJ_FLAG_CLICKABLE);         // 可點擊
    time_menu_im = lv_img_create(time_menu);           // 建立圖片物件
    
    lv_img_set_src(time_menu_im, &timemenu);  // 設定圖片來源為聯絡圖
    lv_obj_set_width(time_menu_im, LV_SIZE_CONTENT);    // 寬度自適應
    lv_obj_set_height(time_menu_im, LV_SIZE_CONTENT);   // 高度自適應
    lv_obj_set_x(time_menu_im, 0);                      // X座標0
    lv_obj_set_y(time_menu_im, 0);                    // Y座標0
    lv_obj_set_align(time_menu_im, LV_ALIGN_CENTER);    // 置中對齊
    lv_obj_add_flag(time_menu_im, LV_OBJ_FLAG_ADV_HITTEST); // 進階點擊測試
    lv_obj_clear_flag(time_menu_im, LV_OBJ_FLAG_SCROLLABLE); // 取消可捲動

    lv_obj_add_event_cb(time_menu, btn_event_cb, LV_EVENT_ALL, NULL); // 綁定事件

    menu_back = lv_img_create(scr_menu); // 創建Logo標籤
    lv_img_set_src(menu_back, &back);  // 設定圖片來源為聯絡圖
    lv_obj_set_width(menu_back, LV_SIZE_CONTENT);    // 寬度自適應
    lv_obj_set_height(menu_back, LV_SIZE_CONTENT);   // 高度自適應
    lv_obj_set_align(menu_back, LV_ALIGN_TOP_LEFT);    
    lv_obj_add_flag(menu_back, LV_OBJ_FLAG_CLICKABLE); // 可點擊
    lv_obj_clear_flag(menu_back, LV_OBJ_FLAG_SCROLLABLE); // 取消可捲動
    lv_obj_add_event_cb(menu_back, menu_back_btn_event, LV_EVENT_CLICKED, NULL);
    
    ver_menu = lv_btn_create(scr_menu);
    lv_obj_set_size(ver_menu, 110, 110);
    lv_obj_align(ver_menu, LV_ALIGN_TOP_MID, 0, 55);
    lv_obj_set_style_bg_opa(ver_menu, LV_OPA_TRANSP, 0);     // 背景透明
    lv_obj_set_style_border_width(ver_menu, 2, 0);           // 邊框寬度 2px
    lv_obj_set_style_border_color(ver_menu, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_radius(ver_menu, 0, 0);                 // 直角，不要圓角
    lv_obj_add_flag(ver_menu, LV_OBJ_FLAG_CLICKABLE);         // 可點擊
   
    ver_menu_im = lv_img_create(ver_menu);           // 建立圖片物件
    lv_img_set_src(ver_menu_im, &ver);  // 設定圖片來源為聯絡圖
    lv_obj_set_width(ver_menu_im, LV_SIZE_CONTENT);    // 寬度自適應
    lv_obj_set_height(ver_menu_im, LV_SIZE_CONTENT);   // 高度自適應
    lv_obj_set_x(ver_menu_im, 0);                      // X座標0
    lv_obj_set_y(ver_menu_im, 0);                    // Y座標0
    lv_obj_set_align(ver_menu_im, LV_ALIGN_CENTER);    // 置中對齊
    lv_obj_add_flag(ver_menu_im, LV_OBJ_FLAG_ADV_HITTEST); // 進階點擊測試
    lv_obj_clear_flag(ver_menu_im, LV_OBJ_FLAG_SCROLLABLE); // 取消可捲動
    lv_obj_add_event_cb(ver_menu,  menu_to_ver_event, LV_EVENT_CLICKED, NULL);

    menu1 = lv_btn_create(scr_menu);
    lv_obj_set_size(menu1, 110, 110);
    lv_obj_align(menu1, LV_ALIGN_TOP_RIGHT, -40, 55);
    lv_obj_set_style_bg_opa(menu1, LV_OPA_TRANSP, 0);     // 背景透明
    lv_obj_set_style_border_width(menu1, 2, 0);           // 邊框寬度 2px
    lv_obj_set_style_border_color(menu1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_radius(menu1, 0, 0);                 // 直角，不要圓角
    lv_obj_add_flag(menu1, LV_OBJ_FLAG_CLICKABLE);         // 可點擊
   

    menu2 = lv_btn_create(scr_menu);
    lv_obj_set_size(menu2, 110, 110);
    lv_obj_align(menu2, LV_ALIGN_BOTTOM_LEFT, 40, 120);
    lv_obj_set_style_bg_opa(menu2, LV_OPA_TRANSP, 0);     // 背景透明
    lv_obj_set_style_border_width(menu2, 2, 0);           // 邊框寬度 2px
    lv_obj_set_style_border_color(menu2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_radius(menu2, 0, 0);                 // 直角，不要圓角
    lv_obj_add_flag(menu2, LV_OBJ_FLAG_CLICKABLE);         // 可點擊
  

    menu3 = lv_btn_create(scr_menu);
    lv_obj_set_size(menu3, 110, 110);
    lv_obj_align(menu3, LV_ALIGN_BOTTOM_MID, 0, -15);
    lv_obj_set_style_bg_opa(menu3, LV_OPA_TRANSP, 0);     // 背景透明
    lv_obj_set_style_border_width(menu3, 2, 0);           // 邊框寬度 2px
    lv_obj_set_style_border_color(menu3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); // 白色邊框
    lv_obj_set_style_radius(menu3, 0, 0);                 // 直角，不要圓角
    lv_obj_add_flag(menu3, LV_OBJ_FLAG_CLICKABLE);         // 可點擊



}