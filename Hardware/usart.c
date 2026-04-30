#include "usart.h"

UsartCtrl_t UsartCtrl;
void usart_initialize(void)
{
    GPIO_Config_T  configStruct;
    USART_Config_T USART_ConfigStruct;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_USART1);

    GPIO_ConfigPinRemap(GPIO_REMAP_USART1);

    configStruct.pin = GPIO_PIN_6;
    configStruct.mode = GPIO_MODE_AF_PP;
    configStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &configStruct);

    configStruct.pin = GPIO_PIN_7;
    configStruct.mode = GPIO_MODE_IN_FLOATING;
    GPIO_Config(GPIOB, &configStruct);

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX_RX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    USART_Config(USART1, &USART_ConfigStruct);

    USART_EnableInterrupt(USART1, USART_INT_IDLE);

    NVIC_EnableIRQRequest(USART1_IRQn, 1, 0);

    DMA_Config_TX(DMA1_Channel4, (uint32_t)&USART1->DATA, (uint32_t)UsartCtrl.TxBuffer, USART_TX_BUFFER_SIZE);
    DMA_Config_RX(DMA1_Channel5, (uint32_t)&USART1->DATA, (uint32_t)UsartCtrl.RxBuffer, USART_RX_BUFFER_SIZE);

    USART_EnableDMA(USART1, USART_DMA_TX_RX);
    USART_ClearStatusFlag(USART1, USART_FLAG_IDLE);

    USART_Enable(USART1);
}

void USART1_IRQHandler(void)
{

    if (USART_ReadStatusFlag(USART1, USART_FLAG_IDLE) == SET)
    {
        //清除空闲中断标志位
        UsartCtrl.RxDataSize = USART1->STS;
        UsartCtrl.RxDataSize = USART1->DATA;

        DMA1_Channel5->CHCFG_B.CHEN = DISABLE;
        UsartCtrl.RxDataSize = USART_RX_BUFFER_SIZE - DMA1_Channel5->CHNDATA;
        DMA1_Channel5->CHNDATA = USART_RX_BUFFER_SIZE;
        DMA1_Channel5->CHCFG_B.CHEN = ENABLE;

        usart1_send_data(UsartCtrl.RxBuffer, UsartCtrl.RxDataSize);

        UsartCtrl.RxDataSize = 0;
        memset(UsartCtrl.RxBuffer, 0, USART_RX_BUFFER_SIZE);
    }
}

void DMA_Config_TX(DMA_Channel_T* channel, uint32_t cpar, uint32_t cmar, uint16_t cndtr)
{
    DMA_Config_T DMA_ConfigStruct;
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);
	
    DMA_ConfigStruct.bufferSize = cndtr;
    DMA_ConfigStruct.dir = DMA_DIR_PERIPHERAL_DST;
    DMA_ConfigStruct.loopMode = DMA_MODE_NORMAL;
    DMA_ConfigStruct.M2M = DMA_M2MEN_DISABLE;
    DMA_ConfigStruct.memoryBaseAddr = cmar;
    DMA_ConfigStruct.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
    DMA_ConfigStruct.memoryInc = DMA_MEMORY_INC_ENABLE;
    DMA_ConfigStruct.peripheralBaseAddr = cpar;
    DMA_ConfigStruct.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    DMA_ConfigStruct.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    DMA_ConfigStruct.priority = DMA_PRIORITY_VERYHIGH;
    DMA_Config(channel, &DMA_ConfigStruct);
}

void DMA_Config_RX(DMA_Channel_T* channel, uint32_t cpar, uint32_t cmar, uint16_t cndtr)
{
    DMA_Config_T DMA_ConfigStruct;
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);
	
    DMA_ConfigStruct.bufferSize = cndtr;
    DMA_ConfigStruct.dir = DMA_DIR_PERIPHERAL_SRC;
    DMA_ConfigStruct.loopMode = DMA_MODE_CIRCULAR;
    DMA_ConfigStruct.M2M = DMA_M2MEN_DISABLE;
    DMA_ConfigStruct.memoryBaseAddr = cmar;
    DMA_ConfigStruct.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
    DMA_ConfigStruct.memoryInc = DMA_MEMORY_INC_ENABLE;
    DMA_ConfigStruct.peripheralBaseAddr = cpar;
    DMA_ConfigStruct.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    DMA_ConfigStruct.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    DMA_ConfigStruct.priority = DMA_PRIORITY_VERYHIGH;
    DMA_Config(channel, &DMA_ConfigStruct);

    DMA_Enable(channel);
}

void DMA_ReEnable(DMA_Channel_T* channel, uint16_t dataNumber)
{
    channel->CHCFG_B.CHEN = DISABLE;    
    channel->CHNDATA = dataNumber;    
    channel->CHCFG_B.CHEN = ENABLE;         
}


void usart1_send_data(uint8_t* data , uint16_t len)
{
	DMA_ClearStatusFlag(DMA1_FLAG_TC4);                
	DMA_Disable(DMA1_Channel4);               
	memcpy(UsartCtrl.TxBuffer, data, len);
	DMA_ReEnable(DMA1_Channel4, len);
}

