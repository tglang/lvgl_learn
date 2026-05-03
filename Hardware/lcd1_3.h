#ifndef __LCD1_3_H__
#define __LCD1_3_H__

#include "Hardware.h"

#define LCD_DATA_PORT GPIOA
#define LCD_DATA_PIN  GPIO_PIN_7

#define LCD_SCL_PORT  GPIOA
#define LCD_SCL_PIN   GPIO_PIN_5

#define LCD_CS_PORT   GPIOA
#define LCD_CS_PIN    GPIO_PIN_4
#define LCD_CS_HIGH() GPIO_SetBit(LCD_CS_PORT, LCD_CS_PIN)
#define LCD_CS_LOW()  GPIO_ResetBit(LCD_CS_PORT, LCD_CS_PIN)

#define LCD_DC_PORT   GPIOA
#define LCD_DC_PIN    GPIO_PIN_6
#define LCD_DC_DATA() GPIO_SetBit(LCD_DC_PORT, LCD_DC_PIN)
#define LCD_DC_CMD()  GPIO_ResetBit(LCD_DC_PORT, LCD_DC_PIN)

#define LCD_RES_PORT  GPIOC
#define LCD_RES_PIN   GPIO_PIN_5
#define LCD_RES_HIGH() GPIO_SetBit(LCD_RES_PORT, LCD_RES_PIN)
#define LCD_RES_LOW()  GPIO_ResetBit(LCD_RES_PORT, LCD_RES_PIN)

#define LCD_BLK_PORT  GPIOC
#define LCD_BLK_PIN   GPIO_PIN_4
#define LCD_BLK_HIGH() GPIO_SetBit(LCD_BLK_PORT, LCD_BLK_PIN)
#define LCD_BLK_LOW()  GPIO_ResetBit(LCD_BLK_PORT, LCD_BLK_PIN)

#define LCD_TE_PORT   GPIOB
#define LCD_TE_PIN    GPIO_PIN_2


void lcd_initialize(void);
void spi_config(void);
void lcd_gpio_config(void);
void Lcd_SendDatas(uint8_t *data, uint32_t length);
void Lcd_SendCMDs(uint8_t *cmd, uint8_t length);



#endif
