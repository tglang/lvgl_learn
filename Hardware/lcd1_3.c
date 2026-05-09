#include "lcd1_3.h"
#include "lcd_fonts.h"

u16 BACK_COLOR = WHITE;   //背景色

void lcd_initialize(void)
{
    spi_config();
    lcd_gpio_config();

    delay_ms(100);

    Lcd_SendCMD(0x36);
    Lcd_SendData8(0x00);
    // Lcd_SendData8(0xC0);
    // Lcd_SendData8(0x70);
    // Lcd_SendData8(0xA0);

    Lcd_SendCMD(0x3A); 
    Lcd_SendData8(0x05);

    Lcd_SendCMD(0xB2);
    Lcd_SendData8(0x0C);
    Lcd_SendData8(0x0C);
    Lcd_SendData8(0x00);
    Lcd_SendData8(0x33);
    Lcd_SendData8(0x33); 

    Lcd_SendCMD(0xB7); 
    Lcd_SendData8(0x35);  

    Lcd_SendCMD(0xBB);
    Lcd_SendData8(0x37);

    Lcd_SendCMD(0xC0);
    Lcd_SendData8(0x2C);

    Lcd_SendCMD(0xC2);
    Lcd_SendData8(0x01);

    Lcd_SendCMD(0xC3);
    Lcd_SendData8(0x12);   

    Lcd_SendCMD(0xC4);
    Lcd_SendData8(0x20);  

    Lcd_SendCMD(0xC6); 
    Lcd_SendData8(0x0F);    

    Lcd_SendCMD(0xD0); 
    Lcd_SendData8(0xA4);
    Lcd_SendData8(0xA1);

    Lcd_SendCMD(0xE0);
    Lcd_SendData8(0xD0);
    Lcd_SendData8(0x04);
    Lcd_SendData8(0x0D);
    Lcd_SendData8(0x11);
    Lcd_SendData8(0x13);
    Lcd_SendData8(0x2B);
    Lcd_SendData8(0x3F);
    Lcd_SendData8(0x54);
    Lcd_SendData8(0x4C);
    Lcd_SendData8(0x18);
    Lcd_SendData8(0x0D);
    Lcd_SendData8(0x0B);
    Lcd_SendData8(0x1F);
    Lcd_SendData8(0x23);

    Lcd_SendCMD(0xE1);
    Lcd_SendData8(0xD0);
    Lcd_SendData8(0x04);
    Lcd_SendData8(0x0C);
    Lcd_SendData8(0x11);
    Lcd_SendData8(0x13);
    Lcd_SendData8(0x2C);
    Lcd_SendData8(0x3F);
    Lcd_SendData8(0x44);
    Lcd_SendData8(0x51);
    Lcd_SendData8(0x2F);
    Lcd_SendData8(0x1F);
    Lcd_SendData8(0x1F);
    Lcd_SendData8(0x20);
    Lcd_SendData8(0x23);

    Lcd_SendCMD(0x21); 

    Lcd_SendCMD(0x11); 

    Lcd_SendCMD(0x29); 

    LCD_Clear(WHITE);
}

void spi_config(void)
{
    GPIO_Config_T configStruct;
    SPI_Config_T  spiConfig;
    DMA_Config_T DMA_ConfigStruct;

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SPI1 | RCM_APB2_PERIPH_GPIOA);

    //SCK
    configStruct.pin = LCD_SCL_PIN;
    configStruct.mode = GPIO_MODE_AF_PP;//GPIO_MODE_OUT_PP
    configStruct.speed = GPIO_SPEED_50MHz; 
    GPIO_Config(LCD_SCL_PORT, &configStruct);

    //MOSI
    configStruct.pin = LCD_DATA_PIN;
    GPIO_Config(LCD_DATA_PORT, &configStruct);

    //CS
    configStruct.pin = LCD_CS_PIN;
    configStruct.mode = GPIO_MODE_OUT_PP;
    GPIO_Config(LCD_CS_PORT, &configStruct);

    LCD_CS_HIGH();

    SPI_ConfigStructInit(&spiConfig);
    spiConfig.baudrateDiv = SPI_BAUDRATE_DIV_2;
    spiConfig.crcPolynomial = 7;
    spiConfig.direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    spiConfig.firstBit = SPI_FIRSTBIT_MSB;
    spiConfig.length = SPI_DATA_LENGTH_8B;
    spiConfig.mode = SPI_MODE_MASTER;
    spiConfig.nss = SPI_NSS_SOFT;
    spiConfig.phase = SPI_CLKPHA_1EDGE;
    spiConfig.polarity = SPI_CLKPOL_LOW;
    SPI_Config(SPI1, &spiConfig);

    
    DMA_ConfigStruct.dir = DMA_DIR_PERIPHERAL_DST;
    DMA_ConfigStruct.loopMode = DMA_MODE_NORMAL;
    DMA_ConfigStruct.M2M = DMA_M2MEN_DISABLE;
    DMA_ConfigStruct.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
    DMA_ConfigStruct.memoryInc = DMA_MEMORY_INC_ENABLE;
    DMA_ConfigStruct.peripheralBaseAddr = (uint32_t)&SPI1->DATA;
    DMA_ConfigStruct.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    DMA_ConfigStruct.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    DMA_ConfigStruct.priority = DMA_PRIORITY_VERYHIGH;
    DMA_Config(DMA1_Channel3, &DMA_ConfigStruct);
    DMA_Disable(DMA1_Channel3);

    SPI_I2S_EnableDMA(SPI1, SPI_I2S_DMA_REQ_TX);
    SPI_Enable(SPI1);
}

void lcd_gpio_config(void)
{
    GPIO_Config_T configStruct;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB | 
                              RCM_APB2_PERIPH_GPIOC);

    //DC
    configStruct.pin = LCD_DC_PIN;
    configStruct.mode = GPIO_MODE_OUT_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(LCD_DC_PORT, &configStruct);

    //RES
    configStruct.pin = LCD_RES_PIN;
    GPIO_Config(LCD_RES_PORT, &configStruct);

    //BLK
    configStruct.pin = LCD_BLK_PIN;
    GPIO_Config(LCD_BLK_PORT, &configStruct);

    //TE
    configStruct.pin = LCD_TE_PIN;
    configStruct.mode = GPIO_MODE_IN_PU;
    GPIO_Config(LCD_TE_PORT, &configStruct);

    LCD_DC_CMD();
    LCD_RES_LOW();
    delay_ms(20);
    LCD_RES_HIGH();
    delay_ms(20);
    LCD_BLK_HIGH();
}

void Lcd_SendData8(uint8_t data)
{
    LCD_DC_DATA();
    LCD_CS_LOW();

    while(SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_TXBE) == RESET);
    SPI1->DATA = data;
    while(SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_BSY) == SET);
    LCD_CS_HIGH();
}

void Lcd_SendData16(uint16_t data)
{
    LCD_DC_DATA();
    LCD_CS_LOW();

    while(SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_TXBE) == RESET);
    SPI1->DATA = (uint8_t)(data >> 8);
    while(SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_BSY) == SET);
    SPI1->DATA = (uint8_t)data;
    while(SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_BSY) == SET);
    LCD_CS_HIGH();
}

void Lcd_SendDatas(uint8_t *data, uint32_t length)
{
    LCD_DC_DATA();
    LCD_CS_LOW();

    DMA1_Channel3->CHCFG_B.CHEN = DISABLE;
    DMA1_Channel3->CHNDATA = length;
    DMA1_Channel3->CHPADDR = (uint32_t)data;
    DMA1_Channel3->CHCFG_B.CHEN = ENABLE;

    while(DMA_ReadStatusFlag(DMA2_FLAG_TC3) == RESET);
    DMA_ClearStatusFlag(DMA2_FLAG_TC3);
 
    LCD_CS_HIGH();
}


void Lcd_SendCMD(uint8_t cmd)
{
    LCD_DC_CMD();
    LCD_CS_LOW();

    while(SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_TXBE) == RESET);
    SPI1->DATA = cmd;
    while(SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_BSY) == SET);
    LCD_CS_HIGH();
}

void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
    Lcd_SendCMD(0x2a);//列地址设置
    Lcd_SendData16(x1);
    Lcd_SendData16(x2);
    Lcd_SendCMD(0x2b);//行地址设置
    Lcd_SendData16(y1);
    Lcd_SendData16(y2);
    Lcd_SendCMD(0x2c);//储存器写
}

void LCD_Clear(u16 Color)
{
    u16 i,j;  	
    LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
    {
        for (j=0;j<LCD_H;j++)
        {
            Lcd_SendData16(Color);	 			 
        }
    }
}

void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	Lcd_SendData16(color);
} 

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode,u16 color)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;  

    if(x>LCD_W-16||y>LCD_H-16)return;	    //设置窗口		   
	num=num-' ';//得到偏移后的值
	LCD_Address_Set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)Lcd_SendData16(color);
				else Lcd_SendData16(BACK_COLOR);
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);//画一个点     
		        temp>>=1; 
		    }
		}
	}   	   	 	  
}

void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0,color);
        x+=8;
        p++;
    }  
}
