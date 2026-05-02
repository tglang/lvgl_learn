#ifndef __SYSCLK_H__
#define __SYSCLK_H__

#include "Hardware.h"


void clock_initialize(void);
void sysclk_initialize(void);
void SysTick_initialize(void);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);

#endif