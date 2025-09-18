#ifndef UI_STEP2_H
#define UI_STEP2_H

#include "lvgl.h"

// === Step1 畫面全域物件宣告 ===
extern lv_obj_t * ui_Step2;          // Step1 畫面物件
extern lv_obj_t * ui_input_label;    // 顯示輸入數字的 Label
extern lv_obj_t * ui_timer_dropdown; // 如果之後要加選單也能用
extern lv_obj_t * ui_numpad;       // 數字鍵盤容器
extern lv_obj_t * enter_btn;     // Enter 按鈕
extern lv_obj_t * enter_label;
extern lv_obj_t * grid;          // 數字鍵盤的 Grid 容器
extern lv_obj_t * btn;        // 數字按鈕陣列
extern lv_obj_t * label;          // 數字按鈕上的文字
extern lv_obj_t * ui_dot_btn;    // 小數點按鈕
extern lv_obj_t * clear_btn;  // 清除按鈕
extern lv_obj_t * clear_label;
extern lv_obj_t * btn0;
extern lv_obj_t * label0;
extern lv_obj_t * ui_back;
// === 初始化函式 ===
extern void ui_Step2_screen_init(void);
extern void ui_Step2_screen_destroy(void);
#endif // UI_STEP1_H
