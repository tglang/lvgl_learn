#ifndef __LCD1_3_H__
#define __LCD1_3_H__

#include "Hardware.h"

#define LCD_DATA_PORT GPIOA
#define LCD_DATA_PIN  GPIO_PIN_7
#define LCD_DATA_HIGH() GPIO_SetBit(LCD_DATA_PORT, LCD_DATA_PIN)
#define LCD_DATA_LOW()  GPIO_ResetBit(LCD_DATA_PORT, LCD_DATA_PIN)

#define LCD_SCL_PORT  GPIOA
#define LCD_SCL_PIN   GPIO_PIN_5
#define LCD_SCL_HIGH() GPIO_SetBit(LCD_SCL_PORT, LCD_SCL_PIN)
#define LCD_SCL_LOW()  GPIO_ResetBit(LCD_SCL_PORT, LCD_SCL_PIN)

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

#define LCD_W 240
#define LCD_H 240


void lcd_initialize(void);
void spi_config(void);
void lcd_gpio_config(void);
void Lcd_SendData8(uint8_t data);
void Lcd_SendData16(uint16_t data);
void Lcd_SendDatas(uint8_t *data, uint32_t length);
void Lcd_SendCMD(uint8_t cmd);
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_Clear(u16 Color);
void LCD_DrawPoint(u16 x,u16 y,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode,u16 color);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color);


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#endif
