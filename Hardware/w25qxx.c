#include "w25qxx.h"

void w25qxx_initialize(void)
{
    w25qxx_spi_config();
    w25qxx_wp_gpio_config();


}

void w25qxx_spi_config(void)
{
    GPIO_Config_T configStruct;
    SPI_Config_T  spiConfig;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_SPI2);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);

    //MISO
    configStruct.pin = W25QXX_MISO_PIN;
    configStruct.mode = GPIO_MODE_AF_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(W25QXX_MISO_PORT, &configStruct);

    //SCK
    configStruct.pin = W25QXX_SCK_PIN;
    configStruct.mode = GPIO_MODE_IN_PU;
    GPIO_Config(W25QXX_SCK_PORT, &configStruct);

    //MOSI
    configStruct.pin = W25QXX_MOSI_PIN;
    GPIO_Config(W25QXX_MOSI_PORT, &configStruct);

    //CS
    configStruct.pin = W25QXX_CS_PIN;
    configStruct.mode = GPIO_MODE_OUT_PP;
    GPIO_Config(W25QXX_CS_PORT, &configStruct);

    W25QXX_CS_HIGH();

    spiConfig.baudrateDiv = SPI_BAUDRATE_DIV_32;
    spiConfig.crcPolynomial = 7;
    spiConfig.direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    spiConfig.firstBit = SPI_FIRSTBIT_LSB;
    spiConfig.length = SPI_DATA_LENGTH_8B;
    spiConfig.mode = SPI_MODE_MASTER;
    spiConfig.nss = SPI_NSS_SOFT;
    spiConfig.phase = SPI_CLKPHA_1EDGE;
    spiConfig.polarity = SPI_CLKPOL_LOW;
    SPI_Config(SPI2, &spiConfig);
    SPI_Enable(SPI2);
}

void w25qxx_wp_gpio_config(void)
{
    GPIO_Config_T configStruct;
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOC);

    configStruct.pin = W25QXX_WP_PIN;
    configStruct.mode = GPIO_MODE_OUT_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(W25QXX_WP_PORT, &configStruct);

    //W25QXX_HARDWARE_WRITE_DISABLE();
    W25QXX_HARDWARE_WRITE_ENABLE();
}

uint8_t spi_swap_byte(uint8_t byte)
{
    // while(SPI_I2S_ReadStatusFlag(SPI2, SPI_FLAG_TXBE) == RESET);
    // SPI_I2S_TxData(SPI2, byte);
    // while (SPI_I2S_ReadStatusFlag(SPI2, SPI_FLAG_RXBNE) == RESET);

    while((SPI2->STS & SPI_FLAG_TXBE) == RESET);
    SPI2->DATA = byte;
    while((SPI2->STS & SPI_FLAG_RXBNE) == RESET);

    return SPI_I2S_RxData(SPI2);
}

void W25qxx_Write_Protect(WriteProtectType status)
{
	W25QXX_CS_LOW();					  // 使能器件
	if(status == WP_DISABLE)spi_swap_byte(W25QXX_WRITE_DISABLE); // 发送写禁用
    else spi_swap_byte(W25QXX_WRITE_ENABLE); // 发送写使能
	W25QXX_CS_HIGH();					  // 取消片选
}

uint8_t W25Qxx_ReadStatusReg(void)
{
    uint8_t byte;

   W25QXX_CS_LOW();				 // 使能器件
	spi_swap_byte(W25QXX_READ_STATUS_REGISTER_1);	 // 发送读取状态寄存器命令
	byte = spi_swap_byte(0XFF); // 读取一个字节
	W25QXX_CS_HIGH();				 // 取消片选
	return byte;
}

void W25Qxx_WriteStatusReg(uint8_t reg)
{
	W25qxx_Write_Protect(WP_DISABLE);				  // SET WEL
	W25QXX_CS_LOW();					  // 使能器件
	spi_swap_byte(W25QXX_WRITE_STATUS_REGISTER); // 发送写状态寄存器命令
	spi_swap_byte(reg);					 // 状态寄存器数据
	W25QXX_CS_HIGH();					  
}

void W25Qxx_Wait_Busy(void)
{
    uint8_t byte;
    do
    {
        byte = W25Qxx_ReadStatusReg();
    } while ((byte & 0x01) == 0x01); // 等待忙标志清零
}

void W25qxx_EraeData(EraseType type, uint32_t Addr)
{
    W25qxx_Write_Protect(WP_DISABLE);
    W25Qxx_Wait_Busy();
    W25QXX_CS_LOW();	

    switch(type)
    {
        case ERASE_SECTOR://擦除扇区4KB
            spi_swap_byte(W25QXX_SECTOR_ERASE_4KB);
            spi_swap_byte((uint8_t)(Addr >> 16));
            spi_swap_byte((uint8_t)(Addr >> 8));
            spi_swap_byte((uint8_t)Addr);
            break;
        case ERASE_BLOCK://擦除块64KB
            spi_swap_byte(W25QXX_BLOCK_ERASE_64KB);
            spi_swap_byte((uint8_t)(Addr >> 16));
            spi_swap_byte((uint8_t)(Addr >> 8));
            spi_swap_byte((uint8_t)Addr);
            break;
        case ERASE_CHIP: spi_swap_byte(W25QXX_CHIP_ERASE); break;//擦除整个芯片
        default: break;
    }

    W25QXX_CS_HIGH();
    W25Qxx_Wait_Busy();
}

void W25Qxx_PowerDown(void)
{
    W25QXX_CS_LOW();					// 使能器件
	spi_swap_byte(W25QXX_POWER_DOWN); // 发送掉电命令
	W25QXX_CS_HIGH();					// 取消片选
	//Delay_us(3);						// 等待TPD
}

void W25Qxx_WAKEUP(void)
{
    W25QXX_CS_LOW();						   // 使能器件
	spi_swap_byte(W25QXX_RELEASE_POWER_DOWN_HPM_DEVICE_ID); //  send W25QXX_RELEASE_POWER_DOWN_HPM_DEVICE_ID command 0xAB
	W25QXX_CS_HIGH();						   // 取消片选
	//Delay_us(3);							   // 等待TRES1
}

uint16_t W25qxx_ReadID(void)
{
    uint16_t temp = 0;
	W25QXX_CS_LOW();
	spi_swap_byte(W25QXX_MANUFACTURER_DEVICE_ID); // 发送读取ID命令
	spi_swap_byte(0x00);
	spi_swap_byte(0x00);
	spi_swap_byte(0x00);
	temp |= spi_swap_byte(0xFF) << 8;
	temp |= spi_swap_byte(0xFF);
	W25QXX_CS_HIGH();

	return temp;
} 

void W25Qxx_ReadData(uint8_t *Buffer, uint32_t ReadAddr, uint32_t Length)
{
    uint32_t i;

	W25QXX_CS_LOW();
	spi_swap_byte(W25QXX_READ_DATA); // 发送读取命令

	spi_swap_byte((uint8_t)(ReadAddr >> 16)); // 发送24bit地址
	spi_swap_byte((uint8_t)(ReadAddr >> 8));
	spi_swap_byte((uint8_t)ReadAddr);
	for (i = 0; i < Length; i++)Buffer[i] = spi_swap_byte(0XFF); // 循环读数
	W25QXX_CS_HIGH();
}

void W25Qxx_Write_Page(uint8_t *Buffer, uint32_t WriteAddr, uint16_t Length)
{
    uint8_t i;

	W25qxx_Write_Protect(WP_DISABLE);				  // SET WEL
	W25QXX_CS_LOW();					  // 使能器件
	spi_swap_byte(W25QXX_PAGE_PROGRAM); // 发送写页命令
	spi_swap_byte((uint8_t)(WriteAddr >> 16)); // 发送24bit地址
	spi_swap_byte((uint8_t)(WriteAddr >> 8));
	spi_swap_byte((uint8_t)WriteAddr);
	for (i = 0; i < Length; i++)spi_swap_byte(Buffer[i]); // 循环写数
	W25QXX_CS_HIGH();					// 取消片选
	W25Qxx_Wait_Busy();					// 等待写入结束
}

void W25Qxx_Write_NoCheck(uint8_t *Buffer, uint32_t WriteAddr, uint32_t Length)
{
	uint16_t Counts;
	Counts = 256 - WriteAddr % 256; // 单页剩余的字节数
	if (Length <= Counts)Counts = Length; // 不大于256个字节
	while (1)
	{
		W25Qxx_Write_Page(Buffer, WriteAddr, Counts);
		if (Length == Counts)break; // 写入结束了
		else// Length > Counts
		{
			Buffer += Counts;
			WriteAddr += Counts;

			Length -= Counts; // 减去已经写入了的字节数
			if (Length > 256)Counts = 256; // 一次可以写入256个字节
			else Counts = Length; // 不够256个字节了
		}
	}
}


uint8_t W25QXX_BUFFER[4096];

void W25Qxx_WriteData(uint8_t *Buffer, uint32_t WriteAddr, uint32_t Length)
{
    uint32_t position, offset, Counts, i;
	uint8_t *W25QXX_BUF;

	W25QXX_BUF = W25QXX_BUFFER;
	position = WriteAddr / 4096; // 扇区地址
	offset = WriteAddr % 4096; // 在扇区内的偏移
	Counts = 4096 - offset; // 扇区剩余空间大小

	if (Length <= Counts)Counts = Length; // 不大于4096个字节
	while (1)
	{
		W25Qxx_ReadData(W25QXX_BUF, position * 4096, 4096); // 读出整个扇区的内容
		for (i = 0; i < Counts; i++)				  // 校验数据
		{
			if (W25QXX_BUF[offset + i] != 0XFF)break; // 需要擦除
		}
		if (i < Counts) // 需要擦除
		{
			W25qxx_EraeData(ERASE_SECTOR, position * 4096);	// 擦除这个扇区
			for (i = 0; i < Counts; i++)W25QXX_BUF[i + offset] = Buffer[i]; // 复制
			W25Qxx_Write_NoCheck(W25QXX_BUF, position * 4096, 4096); // 写入整个扇区
		}
		else W25Qxx_Write_NoCheck(Buffer, WriteAddr, Counts); // 写已经擦除了的,直接写入扇区剩余区间.
		
		if (Length == Counts)break; // 写入结束了
		else	   // 写入未结束
		{
			position++;	// 扇区地址增1
			offset = 0; // 偏移位置为0

			Buffer += Counts;		 // 指针偏移
			WriteAddr += Counts;		 // 写地址偏移
			Length -= Counts; // 字节数递减
			if (Length > 4096)Counts = 4096; // 下一个扇区还是写不完
			else Counts = Length; // 下一个扇区可以写完了
		}
	}
}


