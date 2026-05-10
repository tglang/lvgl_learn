#include "Hardware.h"

#include "../lvgl.h"
#include "lv_port_disp.h"

TimerCtrl_t TimerCtrl;

lv_obj_t *scr;
lv_obj_t *arc;

int main(void)
{
    Hardware_initialize();
    Software_initialize();

    lv_init();
    lv_port_disp_init();

    scr = lv_scr_act(); // 创建屏幕
    // 设置白色背景
    lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN); // 设置背景颜色为白色
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);       // 背景颜色完全不透明

    // 创建圆弧对象
    arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 220, 220);                                                      // 设置圆弧大小
    lv_obj_center(arc);                                                                  // 设置圆弧居中
    lv_arc_set_range(arc, 0, 100);     // 设置圆弧的数值范围为 0-100
    lv_arc_set_value(arc, 50);                                                         // 初始亮度为0

    lv_arc_set_mode(arc, LV_ARC_MODE_REVERSE);

    lv_obj_set_style_arc_width(arc, 6, LV_PART_MAIN);                                    // 设置背景弧线宽度
    lv_obj_set_style_arc_color(arc, lv_color_make(0x57, 0x59, 0x5D), LV_PART_MAIN);      // 设置背景弧线颜色
    lv_obj_set_style_arc_opa(arc, LV_OPA_TRANSP, LV_PART_MAIN);                          // 设置完全透明
    
    lv_obj_set_style_arc_width(arc, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_make(0x57, 0x59, 0x5D), LV_PART_INDICATOR);  // 可以是任意颜色
    // lv_obj_add_flag(arc, LV_OBJ_FLAG_HIDDEN);


    while(1)
    {
        if(TimerCtrl.Flag_1ms)
        {
            TimerCtrl.Flag_1ms = 0;


        }

        if(TimerCtrl.Flag_8ms)
        {
            TimerCtrl.Flag_8ms = 0;

            lv_task_handler();  // 处理LVGL任务

            KeyCtrl.KeyVaule = KeyScan();
            if(KeyCtrl.KeyVaule == 1)KeyCtrl.KeyTask = 1;
            else if (KeyCtrl.KeyVaule == 2)KeyCtrl.KeyTask = 2;
            else if (KeyCtrl.KeyVaule == 3)KeyCtrl.KeyTask = 3;
            else if (KeyCtrl.KeyVaule == 4)KeyCtrl.KeyTask = 4;
        }

        if(TimerCtrl.Flag_128ms)
        {
            TimerCtrl.Flag_128ms = 0;
            if(KeyCtrl.KeyTask == 1)LED1_TOGGLE();
            if (KeyCtrl.KeyTask == 2)LED1_TOGGLE();
            else if (KeyCtrl.KeyTask == 3)LED2_TOGGLE();
            else if (KeyCtrl.KeyTask == 4)LED3_TOGGLE();
        }

        if(TimerCtrl.Flag_1024ms)
        {
            TimerCtrl.Flag_1024ms = 0;

            
        }
    }
}


void TMR5_IRQHandler(void)
{
    if (TMR_ReadIntFlag(TMR5, TMR_INT_UPDATE) == SET)
    {
        TMR_ClearIntFlag(TMR5, TMR_INT_UPDATE);

        lv_tick_inc(1);

        TimerCtrl.Counts++;

        TimerCtrl.Flag_1ms = 1;
        if((TimerCtrl.Counts & 0x000F) == 0)TimerCtrl.Flag_8ms = 1;
        if((TimerCtrl.Counts & 0x007F) == 0)TimerCtrl.Flag_128ms = 1;
        if((TimerCtrl.Counts & 0x03FF) == 0)
        {
            TimerCtrl.Flag_1024ms = 1;
            TimerCtrl.Counts = 0;
        }

        if(KeyCtrl.DelayFlag == 1)KeyCtrl.DelayCount++;
    }
}