#include "lvgl_func.h"

lv_obj_t *scr;
lv_obj_t *arc;
lv_obj_t *label_val;

static void slider_cb(lv_event_t *e)
{
    lv_obj_t *s = lv_event_get_target(e);
    int32_t v = lv_slider_get_value(s);
    char buf[4];
    buf[0] = '0' + v / 100;
    buf[1] = '0' + (v / 10) % 10;
    buf[2] = '0' + v % 10;
    buf[3] = '\0';
    if (buf[0] == '0') buf[0] = ' ';
    lv_arc_set_value(arc, v);
    lv_label_set_text(label_val, buf);
}

static void btn_cb(lv_event_t *e)
{
    lv_arc_set_value(arc, 0);
    lv_label_set_text(label_val, " 0");
    lv_obj_t *s = (lv_obj_t *)lv_event_get_user_data(e);
    lv_slider_set_value(s, 0, LV_ANIM_ON);
}

void LVGL_UI_initialize(void)
{
    lv_init();
    lv_port_disp_init();

    scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    /* 标题 */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "LVGL Test");
    lv_obj_set_style_text_color(title, lv_color_make(0x33, 0x33, 0x33), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

    /* Arc */
    arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 170, 170);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, -10);
    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 50);
    lv_obj_set_style_arc_width(arc, 8, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_make(0xDD, 0xDD, 0xDD), LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_make(0x33, 0x66, 0xCC), LV_PART_INDICATOR);

    /* 中心数值 */
    label_val = lv_label_create(scr);
    lv_label_set_text(label_val, " 50");
    lv_obj_set_style_text_color(label_val, lv_color_make(0x33, 0x33, 0x33), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_val, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_align(label_val, LV_ALIGN_CENTER, 0, -10);

    /* Slider */
    lv_obj_t *slider = lv_slider_create(scr);
    lv_obj_set_size(slider, 160, 10);
    lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -45);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Reset 按钮 */
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_size(btn, 80, 30);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(btn, btn_cb, LV_EVENT_CLICKED, slider);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Reset");
    lv_obj_center(btn_label);
}
