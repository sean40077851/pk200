#ifndef UI_MENU_H
#define UI_MENU_H

#include "lvgl.h"

extern lv_obj_t *scr_menu;     
extern lv_obj_t *time_menu;    
extern lv_obj_t *time_menu_im; 
extern lv_obj_t *menu_back;

extern void btn_event_cb(lv_event_t *e);
extern void ui_menu_screen_init(void);
extern void back_btn_event(lv_event_t * e);

#endif // UI_MENU_H