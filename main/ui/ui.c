// 此文件由 SquareLine Studio 生成
// SquareLine Studio 版本: SquareLine Studio 1.5.3
// LVGL 版本: 8.3.6
// 項目名稱: EV_Charger

#include "ui.h"            // 包含 UI 相關的頭文件
#include "ui_helpers.h"    // 包含 UI 輔助函數的頭文件
#include "esp_log.h"       // 包含 ESP32 日誌系統的頭文件

///////////////////// 變數聲明 ////////////////////
// 以下是動畫函數的聲明，每個函數都會返回一個 lv_anim_t 指針
lv_anim_t * charging1_Animation(lv_obj_t * TargetObject, int delay);    // 充電動畫1
lv_anim_t * charging2_Animation(lv_obj_t * TargetObject, int delay);    // 充電動畫2
lv_anim_t * charging3_Animation(lv_obj_t * TargetObject, int delay);    // 充電動畫3
lv_anim_t * left_on_Animation(lv_obj_t * TargetObject, int delay);      // 左側進入動畫
lv_anim_t * left_off_Animation(lv_obj_t * TargetObject, int delay);     // 左側退出動畫
lv_anim_t * bottom_on_Animation(lv_obj_t * TargetObject, int delay);    // 底部進入動畫
lv_anim_t * bottom_off_Animation(lv_obj_t * TargetObject, int delay);   // 底部退出動畫
lv_anim_t * phone_Animation(lv_obj_t * TargetObject, int delay);        // 手機動畫
lv_anim_t * big_left_on_Animation(lv_obj_t * TargetObject, int delay);  // 大型左側進入動畫
lv_anim_t * ready_on_Animation(lv_obj_t * TargetObject, int delay);     // 準備狀態進入動畫
lv_anim_t * ready_off_Animation(lv_obj_t * TargetObject, int delay);    // 準備狀態退出動畫
lv_anim_t * top_on_Animation(lv_obj_t * TargetObject, int delay);       // 頂部進入動畫
lv_anim_t * top_off_Animation(lv_obj_t * TargetObject, int delay);      // 頂部退出動畫
lv_anim_t * end_bar_on_Animation(lv_obj_t * TargetObject, int delay);   // 結束條進入動畫
lv_anim_t * end_bar_off_Animation(lv_obj_t * TargetObject, int delay);  // 結束條退出動畫
lv_anim_t * light_Animation(lv_obj_t * TargetObject, int delay);        // 燈光動畫

// 事件處理函數聲明
void ui_event____initial_actions0(lv_event_t * e);     // 初始動作事件處理函數
lv_obj_t * ui____initial_actions0;                     // 初始動作物件

// 啟動畫面計時器回調函數
static void splash_timer_cb(lv_timer_t * t)
{
    lv_timer_del(t);                    // 刪除計時器，避免重複執行
    ESP_LOGI("UI", "splash_timer_cb fired");  // 輸出日誌訊息
    ui_Home_screen_init();              // 初始化主畫面
    lv_scr_load(ui_Home);              // 載入主畫面，只執行一次
}

// 圖片和圖片集聲明區域（此處為空）

///////////////////// LVGL 設定測試 ////////////////////
// 檢查顏色深度設定是否符合 SquareLine Studio 的要求
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
// 檢查 16 位顏色交換設定是否符合 SquareLine Studio 的要求
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// 動畫定義 ////////////////////

// 充電動畫1：垂直移動動畫，從上方滑入
lv_anim_t * charging1_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 400);                      // 設定動畫持續時間為 400ms
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);  // 設定 Y 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, -50, 15);                // 設定動畫數值範圍：從 Y=-50 到 Y=15
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot); // 設定過衝動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0（不回放）
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);                // 設定重複次數為 0（不重複）
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, false);             // 設定不提前應用動畫
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y); // 設定獲取 Y 值的回調函數
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 充電動畫2：水平移動動畫，持續循環
lv_anim_t * charging2_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 6000);                     // 設定動畫持續時間為 6000ms (6秒)
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);  // 設定 X 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, 0, 577);                 // 設定動畫數值範圍：從 X=0 到 X=577
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);    // 設定線性動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE); // 設定無限重複
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, false);             // 設定不提前應用動畫
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_x); // 設定獲取 X 值的回調函數
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 充電動畫3：較慢的水平移動動畫，持續循環
lv_anim_t * charging3_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 10000);                    // 設定動畫持續時間為 10000ms (10秒)
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);  // 設定 X 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, 0, 573);                 // 設定動畫數值範圍：從 X=0 到 X=573
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);    // 設定線性動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE); // 設定無限重複
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, false);             // 設定不提前應用動畫
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_x); // 設定獲取 X 值的回調函數
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 左側進入動畫：從左側滑入的過衝動畫
lv_anim_t * left_on_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 600);                      // 設定動畫持續時間為 600ms
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);  // 設定 X 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, -470, -115);             // 設定動畫數值範圍：從 X=-470 到 X=-115
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot); // 設定過衝動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);                // 設定重複次數為 0
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, false);             // 設定不提前應用動畫
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 左側退出動畫：快速向左滑出
lv_anim_t * left_off_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 200);                      // 設定動畫持續時間為 200ms
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);  // 設定 X 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, 0, -200);                // 設定動畫數值範圍：從 X=0 到 X=-200
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);   // 設定緩入動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);                // 設定重複次數為 0
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, false);             // 設定不提前應用動畫
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_x); // 設定獲取 X 值的回調函數
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 底部進入動畫：從下方滑入的過衝動畫
lv_anim_t * bottom_on_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 600);                      // 設定動畫持續時間為 600ms
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);  // 設定 Y 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, 70, 13);                 // 設定動畫數值範圍：從 Y=70 到 Y=13
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot); // 設定過衝動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);                // 設定重複次數為 0
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, false);             // 設定不提前應用動畫
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 底部退出動畫：向下滑出
lv_anim_t * bottom_off_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 300);                      // 設定動畫持續時間為 300ms
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);  // 設定 Y 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, 0, 200);                 // 設定動畫數值範圍：從 Y=0 到 Y=200
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);    // 設定線性動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);                // 設定重複次數為 0
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, false);             // 設定不提前應用動畫
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 手機動畫：從下方滑入的緩出動畫
lv_anim_t * phone_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 600);                      // 設定動畫持續時間為 600ms
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);  // 設定 Y 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, 200, 0);                 // 設定動畫數值範圍：從 Y=200 到 Y=0
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);  // 設定緩出動畫路徑
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);               // 設定動畫延遲時間
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data); // 設定動畫結束後的清理回調
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);               // 設定回放時間為 0
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);              // 設定回放延遲為 0
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);                // 設定重複次數為 0
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);                // 設定重複延遲為 0
    lv_anim_set_early_apply(&PropertyAnimation_0, true);              // 設定提前應用動畫
    out_anim = lv_anim_start(&PropertyAnimation_0);                   // 啟動動畫並返回動畫指針

    return out_anim;                                                   // 返回動畫指針
}

// 大型左側進入動畫：從左側滑入的過衝動畫
lv_anim_t * big_left_on_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;                                              // 動畫指針
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t)); // 分配用戶數據記憶體
    PropertyAnimation_0_user_data->target = TargetObject;              // 設定目標物件
    PropertyAnimation_0_user_data->val = -1;                          // 設定初始值
    lv_anim_t PropertyAnimation_0;                                     // 動畫結構
    lv_anim_init(&PropertyAnimation_0);                               // 初始化動畫
    lv_anim_set_time(&PropertyAnimation_0, 600);                      // 設定動畫持續時間為 600ms
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data); // 設定用戶數據
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);  // 設定 X 軸移動回調函數
    lv_anim_set_values(&PropertyAnimation_0, -200, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    out_anim = lv_anim_start(&PropertyAnimation_0);

    return out_anim;
}
lv_anim_t * ready_on_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 800);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle);
    lv_anim_set_values(&PropertyAnimation_0, 1800, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 100);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    out_anim = lv_anim_start(&PropertyAnimation_0);
    ui_anim_user_data_t * PropertyAnimation_1_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_1_user_data->target = TargetObject;
    PropertyAnimation_1_user_data->val = -1;
    lv_anim_t PropertyAnimation_1;
    lv_anim_init(&PropertyAnimation_1);
    lv_anim_set_time(&PropertyAnimation_1, 600);
    lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_image_zoom);
    lv_anim_set_values(&PropertyAnimation_1, 150, 256);
    lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_1, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_1, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
    lv_anim_set_early_apply(&PropertyAnimation_1, true);
    out_anim = lv_anim_start(&PropertyAnimation_1);
    ui_anim_user_data_t * PropertyAnimation_2_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_2_user_data->target = TargetObject;
    PropertyAnimation_2_user_data->val = -1;
    lv_anim_t PropertyAnimation_2;
    lv_anim_init(&PropertyAnimation_2);
    lv_anim_set_time(&PropertyAnimation_2, 300);
    lv_anim_set_user_data(&PropertyAnimation_2, PropertyAnimation_2_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_2, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_2, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_2, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_2, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_2, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_2, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_2, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_2, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_2, 0);
    lv_anim_set_early_apply(&PropertyAnimation_2, true);
    out_anim = lv_anim_start(&PropertyAnimation_2);

    return out_anim;
}
lv_anim_t * ready_off_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 200);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 255, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    out_anim = lv_anim_start(&PropertyAnimation_0);
    ui_anim_user_data_t * PropertyAnimation_1_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_1_user_data->target = TargetObject;
    PropertyAnimation_1_user_data->val = -1;
    lv_anim_t PropertyAnimation_1;
    lv_anim_init(&PropertyAnimation_1);
    lv_anim_set_time(&PropertyAnimation_1, 200);
    lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_image_zoom);
    lv_anim_set_values(&PropertyAnimation_1, 255, 150);
    lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_1, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_1, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
    lv_anim_set_early_apply(&PropertyAnimation_1, false);
    out_anim = lv_anim_start(&PropertyAnimation_1);
    ui_anim_user_data_t * PropertyAnimation_2_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_2_user_data->target = TargetObject;
    PropertyAnimation_2_user_data->val = -1;
    lv_anim_t PropertyAnimation_2;
    lv_anim_init(&PropertyAnimation_2);
    lv_anim_set_time(&PropertyAnimation_2, 200);
    lv_anim_set_user_data(&PropertyAnimation_2, PropertyAnimation_2_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_2, _ui_anim_callback_set_image_angle);
    lv_anim_set_values(&PropertyAnimation_2, 0, 900);
    lv_anim_set_path_cb(&PropertyAnimation_2, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_2, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_2, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_2, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_2, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_2, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_2, 0);
    lv_anim_set_early_apply(&PropertyAnimation_2, false);
    out_anim = lv_anim_start(&PropertyAnimation_2);

    return out_anim;
}
lv_anim_t * top_on_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 400);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, -100, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    out_anim = lv_anim_start(&PropertyAnimation_0);

    return out_anim;
}
lv_anim_t * top_off_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 200);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, 0, -60);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y);
    out_anim = lv_anim_start(&PropertyAnimation_0);

    return out_anim;
}
lv_anim_t * end_bar_on_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 400);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, 82, 38);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    out_anim = lv_anim_start(&PropertyAnimation_0);

    return out_anim;
}
lv_anim_t * end_bar_off_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 300);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, 38, 200);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    out_anim = lv_anim_start(&PropertyAnimation_0);

    return out_anim;
}
lv_anim_t * light_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 400);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 500);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 400);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    out_anim = lv_anim_start(&PropertyAnimation_0);
    ui_anim_user_data_t * PropertyAnimation_1_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_1_user_data->target = TargetObject;
    PropertyAnimation_1_user_data->val = -1;
    lv_anim_t PropertyAnimation_1;
    lv_anim_init(&PropertyAnimation_1);
    lv_anim_set_time(&PropertyAnimation_1, 400);
    lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_1, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_ease_out);
    lv_anim_set_delay(&PropertyAnimation_1, delay + 2300);
    lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_1, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
    lv_anim_set_early_apply(&PropertyAnimation_1, false);
    out_anim = lv_anim_start(&PropertyAnimation_1);

    return out_anim;
}

///////////////////// FUNCTIONS ////////////////////
void ui_event____initial_actions0(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        charging1_Animation(ui_anim_bg1, 0);
        charging2_Animation(ui_anim_bg2, 0);
        charging3_Animation(ui_anim_bg3, 0);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_basic_init(dispp);
    lv_disp_set_theme(dispp, theme);
    ui_SC_LOGO_screen_init();
    lv_scr_load(ui_SC_LOGO);

    // 延遲 2 秒切換到 Home
    ui_Home_screen_init();
    lv_timer_create(splash_timer_cb, 2000, NULL);
    }

void ui_destroy(void)
{
    ui_Home_screen_destroy();
    ui_Step1_screen_destroy();
    ui_Step2_screen_destroy();
    ui_Connect_screen_destroy();
    ui_Welcome_screen_destroy();
    ui_Charging_screen_destroy();
    ui_End_screen_destroy();
}
