#include "gpio.h"

KeyCtrl_t KeyCtrl;

void gpio_initialize(void)
{
    GPIO_Config_T  configStruct;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB | 
                              RCM_APB2_PERIPH_GPIOC | RCM_APB2_PERIPH_GPIOD | 
                              RCM_APB2_PERIPH_AFIO);
                
    // 禁用JTAG功能，启用SWD，释放PB3为普通IO口
    GPIO_ConfigPinRemap(GPIO_REMAP_SWJ_JTAGDISABLE);

    //LED
    configStruct.pin = LED1_PIN;
    configStruct.mode = GPIO_MODE_OUT_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(LED1_PORT, &configStruct);

    configStruct.pin = LED2_PIN;
    GPIO_Config(LED2_PORT, &configStruct);

    configStruct.pin = LED3_PIN;
    GPIO_Config(LED3_PORT, &configStruct);

    //KEY
    configStruct.pin = KEY2_PIN;
    configStruct.mode = GPIO_MODE_IN_PU;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(KEY2_PORT, &configStruct);

    configStruct.pin = KEY3_PIN;
    GPIO_Config(KEY3_PORT, &configStruct);

    configStruct.pin = KEY4_PIN;
    GPIO_Config(KEY4_PORT, &configStruct);

    //POWER
    configStruct.pin = PWR_CTRL_PIN;
    configStruct.mode = GPIO_MODE_OUT_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(PWR_CTRL_PORT, &configStruct);

    configStruct.pin = PWR_SW_PIN;
    configStruct.mode = GPIO_MODE_IN_FLOATING;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(PWR_SW_PORT, &configStruct);

    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    PWR_CTRL_ON();
}

uint8_t KeyScan(void)
{
    uint8_t keyValue = 0;
    static uint8_t KeyUp = 1;//按键松开标志

    if(KeyUp && (!KEY2 || !KEY3 || !KEY4 || !PWR_SW))
    {
        KeyCtrl.DelayFlag = 1;
        if(KeyCtrl.DelayCount >= 50)
        {
            KeyUp = 0;
            if(!PWR_SW)keyValue = 1;
            else if(!KEY2)keyValue = 2;
            else if(!KEY3)keyValue = 3;
            else if(!KEY4)keyValue = 4;
        }
    }
    else if(KeyUp == 0 && !PWR_SW)
    {
        if(++KeyCtrl.PowerCount == 125)//长按1s关机
        {
            PWR_CTRL_OFF();
            KeyCtrl.PowerCount = 0;
        }
    }
    else if(KEY2 && KEY3 && KEY4 && PWR_SW)
    {
        KeyUp = 1;
        KeyCtrl.DelayFlag = 0;
        KeyCtrl.DelayCount = 0;
        KeyCtrl.PowerCount = 0;
    }

    return keyValue;
}
