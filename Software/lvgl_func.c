#include "lvgl_func.h"
#include "lv_port_indev.h"

lv_obj_t *scr;
lv_obj_t *btn1;
lv_obj_t *btn2;
lv_obj_t *btn3;
lv_obj_t *slider1;

static lv_group_t *group;

/* 按键事件回调函数 */
static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED) {
        /* 按钮被按下：切换背景色作为反馈 */
        static lv_color_t color_on  = {.full = 0x07E0};  /* 绿色 */
        static lv_color_t color_off = {.full = 0xF800};  /* 红色 */

        if(lv_obj_get_style_bg_color(btn, LV_PART_MAIN).full == color_off.full) {
            lv_obj_set_style_bg_color(btn, color_on, LV_PART_MAIN);
        } else {
            lv_obj_set_style_bg_color(btn, color_off, LV_PART_MAIN);
        }
    }
    else if(code == LV_EVENT_FOCUSED) {
        /* 获得焦点：高亮边框 */
        lv_obj_set_style_border_color(btn, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_border_width(btn, 3, LV_PART_MAIN);
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        /* 失去焦点：恢复边框 */
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    }
}

void LVGL_UI_initialize(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();   /* 初始化输入设备（keypad） */

    scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_palette_main(LV_PALETTE_RED), LV_PART_SCROLLBAR);

    /* ── 创建 group 用于按键导航 ── */
    group = lv_group_create();
    lv_group_set_default(group);

    /* ── 创建按钮 1 ── */
    btn1 = lv_btn_create(scr);
    lv_obj_set_pos(btn1, 50, 50);
    lv_obj_set_size(btn1, 80, 40);
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0xF80000), LV_PART_MAIN);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_ALL, NULL);
    lv_group_add_obj(group, btn1);

    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "BTN1");
    lv_obj_center(label1);

    /* ── 创建按钮 2 ── */
    btn2 = lv_btn_create(scr);
    lv_obj_set_pos(btn2, 50, 110);
    lv_obj_set_size(btn2, 80, 40);
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0x00A0F0), LV_PART_MAIN);
    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_ALL, NULL);
    lv_group_add_obj(group, btn2);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "BTN2");
    lv_obj_center(label2);

    /* ── 创建按钮 3 ── */
    btn3 = lv_btn_create(scr);
    lv_obj_set_pos(btn3, 50, 170);
    lv_obj_set_size(btn3, 80, 40);
    lv_obj_set_style_bg_color(btn3, lv_color_hex(0x00C000), LV_PART_MAIN);
    lv_obj_add_event_cb(btn3, btn_event_cb, LV_EVENT_ALL, NULL);
    lv_group_add_obj(group, btn3);

    lv_obj_t *label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "BTN3");
    lv_obj_center(label3);

    /* ── 将 group 绑定到 keypad 输入设备 ── */
    lv_indev_set_group(indev_keypad, group);

    /* 默认聚焦第一个按钮 */
    lv_group_focus_obj(btn1);
}
