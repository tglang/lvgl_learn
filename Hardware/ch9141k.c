#include "ch9141k.h"

BleCtrl_t BleCtrl;

void ch9141k_initializes(void)
{
    ch9141k_usart_config();
    ch9141k_gpio_config();

    BLE_WAKE_UP();
    BLE_AT_MODE();

    
}

void USART2_IRQHandler(void)
{
    if (USART_ReadStatusFlag(USART2, USART_FLAG_IDLE) == SET)
    {
        //清除空闲中断标志位
        BleCtrl.RxDataSize = USART2->STS;
        BleCtrl.RxDataSize = USART2->DATA;

        DMA1_Channel6->CHCFG_B.CHEN = DISABLE;
        BleCtrl.RxDataSize = BLE_RX_BUFFER_SIZE - DMA1_Channel6->CHNDATA;
        DMA1_Channel6->CHNDATA = BLE_RX_BUFFER_SIZE;
        DMA1_Channel6->CHCFG_B.CHEN = ENABLE;

        //ble_send_data(BleCtrl.RxBuffer, BleCtrl.RxDataSize);

        BleCtrl.RxDataSize = 0;
        memset(BleCtrl.RxBuffer, 0, BLE_RX_BUFFER_SIZE);
    }
}


void ch9141k_usart_config(void)
{
    GPIO_Config_T  configStruct;
    USART_Config_T USART_ConfigStruct;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART2);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    configStruct.pin = GPIO_PIN_2;
    configStruct.mode = GPIO_MODE_AF_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &configStruct);

    configStruct.pin = GPIO_PIN_3;
    configStruct.mode = GPIO_MODE_IN_FLOATING;
    GPIO_Config(GPIOA, &configStruct);

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX_RX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    USART_Config(USART2, &USART_ConfigStruct);

    USART_EnableInterrupt(USART2, USART_INT_IDLE);

    NVIC_EnableIRQRequest(USART2_IRQn, 1, 0);

    DMA_Config_TX(DMA1_Channel7, (uint32_t)&USART2->DATA, (uint32_t)BleCtrl.TxBuffer, BLE_TX_BUFFER_SIZE);
    DMA_Config_RX(DMA1_Channel6, (uint32_t)&USART2->DATA, (uint32_t)BleCtrl.RxBuffer, BLE_RX_BUFFER_SIZE);

    USART_EnableDMA(USART2, USART_DMA_TX_RX);
    USART_ClearStatusFlag(USART2, USART_FLAG_IDLE);

    USART_Enable(USART2);
}

void ch9141k_gpio_config(void)
{
    GPIO_Config_T  configStruct;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    configStruct.pin = BLE_SLEEP_PIN;
    configStruct.mode = GPIO_MODE_AF_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(BLE_SLEEP_PORT, &configStruct);

    configStruct.pin = BLE_MODE_PIN;
    GPIO_Config(BLE_MODE_PORT, &configStruct);
}

void ble_send_data(uint8_t* data , uint16_t len)
{
	DMA_ClearStatusFlag(DMA1_FLAG_TC7);                
	DMA_Disable(DMA1_Channel7);               
	memcpy(BleCtrl.TxBuffer, data, len);
	DMA_ReEnable(DMA1_Channel7, len);
}
