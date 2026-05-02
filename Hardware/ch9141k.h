#ifndef __CH9141K_H__
#define __CH9141K_H__

#include "Hardware.h"

#define BLE_SLEEP_PORT GPIOA
#define BLE_SLEEP_PIN  GPIO_PIN_0
#define BLE_FALL_ASLEEP() GPIO_ResetBit(BLE_SLEEP_PORT, BLE_SLEEP_PIN)
#define BLE_WAKE_UP() GPIO_SetBit(BLE_SLEEP_PORT, BLE_SLEEP_PIN)


#define BLE_MODE_PORT GPIOA
#define BLE_MODE_PIN GPIO_PIN_1
#define BLE_TRANSPARENT_MODE() GPIO_SetBit(BLE_MODE_PORT, BLE_MODE_PIN)
#define BLE_AT_MODE()          GPIO_ResetBit(BLE_MODE_PORT, BLE_MODE_PIN)


void ch9141k_initializes(void);
void ch9141k_usart_config(void);
void ch9141k_gpio_config(void);
void ble_send_data(uint8_t* data , uint16_t len);



#endif
