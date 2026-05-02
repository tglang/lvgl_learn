#ifndef __W25QXX_H__
#define __W25QXX_H__ 

#include "Hardware.h"

#define W25QXX_SCK_PORT  GPIOB
#define W25QXX_SCK_PIN   GPIO_PIN_13

#define W25QXX_MISO_PORT GPIOB
#define W25QXX_MISO_PIN  GPIO_PIN_14

#define W25QXX_MOSI_PORT GPIOB
#define W25QXX_MOSI_PIN  GPIO_PIN_15

#define W25QXX_CS_PORT   GPIOB
#define W25QXX_CS_PIN    GPIO_PIN_12
#define W25QXX_CS_LOW()  GPIO_ResetBit(W25QXX_CS_PORT, W25QXX_CS_PIN)
#define W25QXX_CS_HIGH() GPIO_SetBit(W25QXX_CS_PORT, W25QXX_CS_PIN)

#define W25QXX_WP_PORT   GPIOC
#define W25QXX_WP_PIN    GPIO_PIN_6
#define W25QXX_HARDWARE_WRITE_DISABLE() GPIO_ResetBit(W25QXX_WP_PORT, W25QXX_WP_PIN)
#define W25QXX_HARDWARE_WRITE_ENABLE()  GPIO_SetBit(W25QXX_WP_PORT, W25QXX_WP_PIN)

#define W25QXX_WRITE_ENABLE							0x06
#define W25QXX_WRITE_DISABLE						0x04
#define W25QXX_READ_STATUS_REGISTER_1				0x05
#define W25QXX_WRITE_STATUS_REGISTER				0x01
#define W25QXX_PAGE_PROGRAM							0x02
#define W25QXX_QUAD_PAGE_PROGRAM					0x32
#define W25QXX_BLOCK_ERASE_64KB						0xD8
#define W25QXX_SECTOR_ERASE_4KB						0x20
#define W25QXX_CHIP_ERASE							0xC7
#define W25QXX_ERASE_SUSPEND						0x75
#define W25QXX_ERASE_RESUME							0x7A
#define W25QXX_POWER_DOWN							0xB9
#define W25QXX_HIGH_PERFORMANCE_MODE				0xA3
#define W25QXX_CONTINUOUS_READ_MODE_RESET			0xFF
#define W25QXX_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB
#define W25QXX_MANUFACTURER_DEVICE_ID				0x90
#define W25QXX_READ_UNIQUE_ID						0x4B
#define W25QXX_JEDEC_ID								0x9F
#define W25QXX_READ_DATA							0x03
#define W25QXX_FAST_READ							0x0B
#define W25QXX_FAST_READ_DUAL_OUTPUT				0x3B
#define W25QXX_FAST_READ_DUAL_IO					0xBB
#define W25QXX_FAST_READ_QUAD_OUTPUT				0x6B
#define W25QXX_FAST_READ_QUAD_IO					0xEB
#define W25QXX_OCTAL_WORD_READ_QUAD_IO				0xE3

typedef enum
{
    WP_DISABLE = 0,
    WP_ENABLE = 1
}WriteProtectType;

typedef enum
{
    ERASE_SECTOR = 0,
    ERASE_BLOCK = 1,
    ERASE_CHIP = 2
}EraseType;


void w25qxx_initialize(void);
void w25qxx_spi_config(void);
void w25qxx_wp_gpio_config(void);
uint8_t spi_swap_byte(uint8_t byte);
void W25qxx_WriteProtect(WriteProtectType status);
uint8_t W25Qxx_ReadStatusReg(void);
void W25Qxx_WriteStatusReg(uint8_t reg);
void W25Qxx_Wait_Busy(void);
void W25qxx_EraeData(EraseType type, uint32_t Addr);
void W25Qxx_PowerDown(void);
void W25Qxx_WAKEUP(void);
uint16_t W25qxx_ReadID(void);
void W25Qxx_ReadData(uint8_t *Buffer, uint32_t ReadAddr, uint32_t Length);
void W25Qxx_Write_Page(uint8_t *Buffer, uint32_t WriteAddr, uint16_t Length);
void W25Qxx_Write_NoCheck(uint8_t *Buffer, uint32_t WriteAddr, uint32_t Length);
void W25Qxx_WriteData(uint8_t *Buffer, uint32_t WriteAddr, uint32_t Length);


#endif