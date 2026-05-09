#include "Hardware.h"


void Hardware_initialize(void)
{
    clock_initialize();
    delay_ms(50);
    gpio_initialize();
    usart_initialize();
    //ch9141k_initializes();
    //w25qxx_initialize();

    lcd_initialize();
    timer_initialize();
}

