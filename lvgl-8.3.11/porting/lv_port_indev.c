/**
 * @file lv_port_indev.c
 *
 * LVGL 输入设备移植 —— 按键 (Keypad)
 *
 * 将 KeyScan() 扫描到的物理按键映射为 LVGL 导航键，通过 group 机制
 * 在 UI 对象之间切换焦点并触发事件。
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "Hardware.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static uint32_t keypad_get_key(void);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_keypad;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /*------------------
     * Keypad
     * -----------------*/

    /* 初始化按键（GPIO 已在 gpio_initialize() 中配置） */
    keypad_init();

    /* 注册 keypad 输入设备 */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /*
     * 外部需要通过 lv_indev_set_group(indev_keypad, group) 将 group
     * 绑定到此输入设备，之后即可使用 KEY2/KEY3 在 group 中导航、
     * KEY4 触发 LV_EVENT_CLICKED。
     */
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Keypad
 * -----------------*/

/* 初始化 keypad（GPIO 已在上层完成） */
static void keypad_init(void)
{
    /* 无需额外初始化，按键 GPIO 已在 gpio_initialize() 中配置 */
}

/*
 * 由 lv_timer_handler() 每 16ms 调用一次，读取当前按键状态。
 *
 * 注意：不能用 KeyVaule（仅在一个 8ms 周期内非零，会和 16ms 的
 * keypad_read 形成时序竞争，丢键率 ~50%）。这里改用 KeyTask——
 * main.c 的 8ms 处理中已将 KeyVaule 转换为持久化的 KeyTask，
 * 读取后清零即可。
 */
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint32_t last_key = 0;

    uint8_t key_task = KeyCtrl.KeyTask;

    if (key_task != 0) {
        data->state = LV_INDEV_STATE_PR;

        /*
         * 物理按键 → LVGL 键码映射：
         *   KEY1 (PWR_SW, PB10) → LV_KEY_ESC   返回 / 退出
         *   KEY2 (PA15)          → LV_KEY_PREV  上一个
         *   KEY3 (PC10)          → LV_KEY_NEXT  下一个
         *   KEY4 (PC11)          → LV_KEY_ENTER 确认 / 点击
         */
        switch (key_task) {
            case 1:
                last_key = LV_KEY_ESC;
                break;
            case 2:
                last_key = LV_KEY_PREV;
                break;
            case 3:
                last_key = LV_KEY_NEXT;
                break;
            case 4:
                last_key = LV_KEY_ENTER;
                break;
            default:
                break;
        }

        KeyCtrl.KeyTask = 0;    /* 消费本次按键事件 */
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

/* 返回当前按下的按键值（由 keypad_read 内部 latch 处理，此处保留以备扩展） */
static uint32_t keypad_get_key(void)
{
    return 0;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
