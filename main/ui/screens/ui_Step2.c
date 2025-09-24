#include "lvgl.h"
#include <stdio.h>
#include "esp_log.h"
#include "../ui.h"  // 包含UI頭文件
#include "my_mqtt.h" // 包含 MQTT 頭文件
lv_obj_t * ui_input_label;   // 顯示輸入數字用
static char input_buf[16] = {0};    // 存輸入的數字
#define TAG "UI_Step2"

lv_obj_t * ui_Step2; // Step2 畫面物件
lv_obj_t * grid;          // Step1 畫面物件
lv_obj_t * btn; 
lv_obj_t * label;
lv_obj_t * enter_btn;
lv_obj_t * enter_label;

lv_obj_t * clear_btn;
lv_obj_t * clear_label;
lv_obj_t * btn0;
lv_obj_t * label0;
lv_obj_t * ui_back;


// 處理數字按鈕事件
void num_btn_event(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    const char * txt = lv_label_get_text(lv_obj_get_child(btn, 0));

    if(strlen(input_buf) < sizeof(input_buf)-1) {
            strcat(input_buf, txt);  // 把數字加到字串後面
            lv_label_set_text(ui_input_label, input_buf);

}
}

// Enter 鍵事件
void enter_btn_event(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG_USER("輸入完成: %s", input_buf);

    // 把輸入的字串 Publish 到 MQTT broker
    if (mqtt_handle) {
        esp_mqtt_client_publish(mqtt_handle,"test/mqtt_topic",   // 發佈的 Topic
                                input_buf,           // 發佈的訊息內容
                                0,                   // 自動算長度
                                1,                   // QoS = 1
                                0);                  // retain = 0
        LV_LOG_USER("已發送到 MQTT: %s", input_buf);
    } else {
        LV_LOG_USER("MQTT 尚未初始化");
    }
    input_buf[0] = '\0';  // 清空輸入
    lv_label_set_text(ui_input_label, "");
}

// 返回鍵事件
void back_btn_event(lv_event_t * e)
{
    lv_scr_load(ui_Home);  
}
void clear_btn_event(lv_event_t * e)
{
    LV_UNUSED(e);
    input_buf[0] = '\0';  // 清空輸入
    lv_label_set_text(ui_input_label, "");
    ESP_LOGI(TAG, "已清除輸入");
}


void ui_Step2_screen_init(void)
{
    ui_Step2 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ui_Step2, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(ui_Step2, LV_OPA_COVER, 0);
    lv_obj_clear_flag(ui_Step2, LV_OBJ_FLAG_SCROLLABLE);
    // 顯示輸入框
    ui_input_label = lv_label_create(ui_Step2);
    lv_label_set_text(ui_input_label, "");
    lv_obj_set_style_text_font(ui_input_label, &lv_font_montserrat_28, 0);
    lv_obj_align(ui_input_label, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_text_color(ui_input_label, lv_color_white(), 0); // 白字

    // 建立 Grid
    static lv_coord_t col_dsc[] = {80, 80, 80, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {60, 60, 60, 60, LV_GRID_TEMPLATE_LAST};

    grid = lv_obj_create(ui_Step2);

    lv_obj_set_size(grid, 260, 260);
    lv_obj_center(grid);

    lv_obj_set_style_bg_color(grid, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);  // grid 透明

    lv_obj_set_x(grid, 0);
    lv_obj_set_y(grid, 40);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

    // 建立數字按鍵 (1~9)
    int num = 1;
    for(int row=0; row<3; row++) {
        for(int col=0; col<3; col++) {
            btn = lv_btn_create(grid);
            lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1,
                                        LV_GRID_ALIGN_STRETCH, row, 1);
            lv_obj_set_style_bg_color(btn, lv_color_black(), 0);
            lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);   // 黑色按鈕背景
            lv_obj_set_style_border_color(btn, lv_color_white(), 0); // 白色邊框
            lv_obj_set_style_border_width(btn, 1, 0);        // 邊框寬度
            label = lv_label_create(btn);
            char buf[2];
            sprintf(buf, "%d", num++);
            lv_label_set_text(label, buf);
            lv_obj_set_style_text_color(label, lv_color_white(), 0);  // 白字
            lv_obj_center(label);
            lv_obj_add_event_cb(btn, num_btn_event, LV_EVENT_CLICKED, NULL);

        }
    }

    // Enter 鍵
    enter_btn = lv_btn_create(grid);
    lv_obj_set_grid_cell(enter_btn, LV_GRID_ALIGN_STRETCH, 0, 1,
                                    LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_set_style_bg_opa(enter_btn, LV_OPA_COVER, 0);   // 黑色按鈕背景
    lv_obj_set_style_bg_color(enter_btn, lv_color_black(), 0);
    lv_obj_set_style_border_color(enter_btn, lv_color_white(), 0); // 白色邊框
    lv_obj_set_style_border_width(enter_btn, 1, 0);        // 邊框寬度
    
    enter_label = lv_label_create(enter_btn);
    lv_label_set_text(enter_label, "Enter");
    lv_obj_set_style_text_color(enter_label, lv_color_white(), 0); // 白字
    lv_obj_center(enter_label);
    lv_obj_add_event_cb(enter_btn, enter_btn_event, LV_EVENT_CLICKED, NULL);

    btn0 = lv_btn_create(grid);
    lv_obj_set_grid_cell(btn0,
                        LV_GRID_ALIGN_STRETCH, 1, 1,
                        LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_set_style_bg_color(btn0, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(btn0, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(btn0, lv_color_white(), 0);
    lv_obj_set_style_border_width(btn0, 1, 0);

    label0 = lv_label_create(btn0);
    lv_label_set_text(label0, "0");
    lv_obj_set_style_text_color(label0, lv_color_white(), 0);
    lv_obj_center(label0);
    lv_obj_add_event_cb(btn0, num_btn_event, LV_EVENT_CLICKED, NULL);

    // 清除鍵
    clear_btn = lv_btn_create(grid);
    lv_obj_set_grid_cell(clear_btn, LV_GRID_ALIGN_STRETCH, 2, 1,
                                    LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_set_style_bg_color(clear_btn, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(clear_btn, LV_OPA_COVER, 0);   // 黑色按鈕背景
    lv_obj_set_style_border_color(clear_btn, lv_color_white(), 0); // 白色邊框
    lv_obj_set_style_border_width(clear_btn, 1, 0);        // 邊框寬度
    
    clear_label = lv_label_create(clear_btn);
    lv_label_set_text(clear_label, "Clear");
    lv_obj_set_style_text_color(clear_label, lv_color_white(), 0); // 白字
    lv_obj_center(clear_label);
    lv_obj_add_event_cb(clear_btn, clear_btn_event, LV_EVENT_CLICKED, NULL);
    
    ui_back = lv_img_create(ui_Step2); // 創建Logo標籤
    lv_img_set_src(ui_back, &back);  // 設定圖片來源為聯絡圖
    lv_obj_set_width(ui_back, LV_SIZE_CONTENT);    // 寬度自適應
    lv_obj_set_height(ui_back, LV_SIZE_CONTENT);   // 高度自適應
    lv_obj_set_align(ui_back, LV_ALIGN_TOP_LEFT);    // 置中對齊
    lv_obj_add_flag(ui_back, LV_OBJ_FLAG_CLICKABLE); // 可點擊
    lv_obj_clear_flag(ui_back, LV_OBJ_FLAG_SCROLLABLE); // 取消可捲動
    lv_obj_add_event_cb(ui_back, back_btn_event, LV_EVENT_CLICKED, NULL);
    
    // 最後才 load 畫面
    lv_scr_load(ui_Step2);
}
void ui_Step2_screen_destroy(void)
{
    if(ui_Step2) lv_obj_del(ui_Step2);
    ui_Step2 = NULL;
    grid = NULL;
    btn = NULL;
    label = NULL;
    enter_btn = NULL;
    enter_label = NULL;
    clear_btn = NULL;
    clear_label = NULL;
    ui_back = NULL;

}