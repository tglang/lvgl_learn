#ifndef __Data_H__
#define __Data_H__

#include "Hardware.h"

#define USART_RX_BUFFER_SIZE 50
#define USART_TX_BUFFER_SIZE 50

#define BLE_RX_BUFFER_SIZE 50
#define BLE_TX_BUFFER_SIZE 50

typedef struct 
{
    uint8_t  RxBuffer[USART_RX_BUFFER_SIZE];
    uint16_t RxDataSize;
    uint8_t  TxBuffer[USART_TX_BUFFER_SIZE];
}UsartCtrl_t;

extern UsartCtrl_t UsartCtrl;

typedef struct 
{
    uint8_t  RxBuffer[BLE_RX_BUFFER_SIZE];
    uint16_t RxDataSize;
    uint8_t  TxBuffer[BLE_TX_BUFFER_SIZE];
}BleCtrl_t;

extern BleCtrl_t BleCtrl;

typedef struct 
{
    uint16_t Counts;
    uint8_t  Flag_1ms;
    uint8_t  Flag_8ms;
    uint8_t  Flag_128ms;
    uint8_t  Flag_1024ms;
}TimerCtrl_t;

extern TimerCtrl_t TimerCtrl;

typedef struct 
{
    uint8_t  DelayFlag;
    uint16_t DelayCount;
    uint8_t  PowerCount;
    uint8_t  KeyVaule;
    uint8_t  KeyTask;
}KeyCtrl_t;

extern KeyCtrl_t KeyCtrl;


#endif
