#ifndef __gpio_H__
#define __gpio_H__

#include "Hardware.h"

#define LED1_PORT  GPIOC
#define LED1_PIN   GPIO_PIN_12
#define LED1_ON()     GPIO_SetBit(LED1_PORT, LED1_PIN)
#define LED1_OFF()    GPIO_ResetBit(LED1_PORT, LED1_PIN)
#define LED1_TOGGLE() LED1_PORT->ODATA ^= (uint32_t)LED1_PIN

#define LED2_PORT  GPIOD
#define LED2_PIN   GPIO_PIN_2
#define LED2_ON()     GPIO_SetBit(LED2_PORT, LED2_PIN)
#define LED2_OFF()    GPIO_ResetBit(LED2_PORT, LED2_PIN)
#define LED2_TOGGLE() LED2_PORT->ODATA ^= (uint32_t)LED2_PIN

#define LED3_PORT  GPIOB
#define LED3_PIN   GPIO_PIN_3
#define LED3_ON()     GPIO_SetBit(LED3_PORT, LED3_PIN)
#define LED3_OFF()    GPIO_ResetBit(LED3_PORT, LED3_PIN)
#define LED3_TOGGLE() LED3_PORT->ODATA ^= (uint32_t)LED3_PIN


#define KEY2_PORT  GPIOA
#define KEY2_PIN   GPIO_PIN_15
#define KEY2       (KEY2_PORT->IDATA & KEY2_PIN)

#define KEY3_PORT  GPIOC
#define KEY3_PIN   GPIO_PIN_10
#define KEY3       (KEY3_PORT->IDATA & KEY3_PIN)

#define KEY4_PORT  GPIOC
#define KEY4_PIN   GPIO_PIN_11
#define KEY4       (KEY4_PORT->IDATA & KEY4_PIN)


#define PWR_SW_PORT GPIOB
#define PWR_SW_PIN  GPIO_PIN_10
#define PWR_SW      (PWR_SW_PORT->IDATA & PWR_SW_PIN)

#define PWR_CTRL_PORT GPIOB 
#define PWR_CTRL_PIN  GPIO_PIN_11
#define PWR_CTRL_ON()     GPIO_SetBit(PWR_CTRL_PORT, PWR_CTRL_PIN)
#define PWR_CTRL_OFF()    GPIO_ResetBit(PWR_CTRL_PORT, PWR_CTRL_PIN)


void gpio_initialize(void);
uint8_t KeyScan(void);


#endif
