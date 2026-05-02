#include "Hardware.h"


void Hardware_initialize(void)
{
    clock_initialize();
    gpio_initialize();
    usart_initialize();
    //ch9141k_initializes();
    //w25qxx_initialize();

    timer_initialize();
}

