#include "lcd1_3.h"

void lcd_initialize(void)
{
    spi_config();
    lcd_gpio_config();
}

void spi_config(void)
{
    GPIO_Config_T configStruct;
    SPI_Config_T  spiConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SPI1 | RCM_APB2_PERIPH_GPIOA);

    //SCK
    configStruct.pin = LCD_SCL_PIN;
    configStruct.mode = GPIO_MODE_AF_PP;
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

    spiConfig.baudrateDiv = SPI_BAUDRATE_DIV_32;
    spiConfig.crcPolynomial = 7;
    spiConfig.direction = SPI_DIRECTION_1LINE_TX;
    spiConfig.firstBit = SPI_FIRSTBIT_LSB;
    spiConfig.length = SPI_DATA_LENGTH_8B;
    spiConfig.mode = SPI_MODE_MASTER;
    spiConfig.nss = SPI_NSS_SOFT;
    spiConfig.phase = SPI_CLKPHA_1EDGE;
    spiConfig.polarity = SPI_CLKPOL_LOW;
    SPI_Config(SPI1, &spiConfig);
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


void Lcd_SendDatas(uint8_t *data, uint32_t length)
{
    uint32_t i;

    LCD_CS_LOW();
    LCD_DC_DATA();

    while((SPI1->STS & SPI_FLAG_TXBE) == RESET);
    for(i = 0; i < length; i++)
    {
        SPI1->DATA = data[i];
        while((SPI1->STS & SPI_FLAG_TXBE) == RESET);
    }
 
    LCD_CS_HIGH();
}


void Lcd_SendCMDs(uint8_t *cmd, uint8_t length)
{
    LCD_CS_LOW();
    LCD_DC_CMD();

    while((SPI1->STS & SPI_FLAG_TXBE) == RESET);
    for(int i = 0; i < length; i++)
    {
        SPI1->DATA = cmd[i];
        while((SPI1->STS & SPI_FLAG_TXBE) == RESET);
    }

    LCD_CS_HIGH();
}

