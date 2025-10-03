#include "../ui.h"  // 包含UI頭文件


lv_obj_t *scr_menu=NULL;
lv_obj_t *time_menu=NULL;
lv_obj_t *time_menu_im=NULL;
lv_obj_t *menu_back=NULL;

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
   /* // 建立第二顆按鈕
    lv_obj_t *btn2 = lv_btn_create(scr1);
    lv_obj_set_size(btn2, 80, 80);
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0xff4444), 0);
    lv_obj_align_to(btn2, btn1, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "BTN2");
    lv_obj_center(label2);

    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_ALL, NULL);

    // 測試可捲動：再多放幾顆按鈕往下排
    for(int i=0; i<10; i++) {
        lv_obj_t *btn = lv_btn_create(scr1);
        lv_obj_set_size(btn, 80, 80);
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10, 100 + i*90);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text_fmt(lbl, "BTN%d", i+3);
        lv_obj_center(lbl);
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
    }*/
}