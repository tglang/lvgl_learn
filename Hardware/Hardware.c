#include "Hardware.h"


void Hardware_initialize(void)
{
    gpio_initialize();
    usart_initialize();


    timer_initialize();
}