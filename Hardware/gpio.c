#include "gpio.h"

void gpio_initialize(void)
{
    GPIO_Config_T  configStruct;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOC);

    configStruct.pin = GPIO_PIN_12;
    configStruct.mode = GPIO_MODE_OUT_PP;
    configStruct.speed = GPIO_SPEED_50MHz;

    GPIO_Config(GPIOC, &configStruct);
}


