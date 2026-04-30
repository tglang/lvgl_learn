#ifndef __usart_H__
#define __usart_H__

#include "Hardware.h"



void usart_initialize(void);
void DMA_Config_TX(DMA_Channel_T* channel, uint32_t cpar, uint32_t cmar, uint16_t cndtr);
void DMA_Config_RX(DMA_Channel_T* channel, uint32_t cpar, uint32_t cmar, uint16_t cndtr);
void DMA_ReEnable(DMA_Channel_T* channel, uint16_t dataNumber);
void usart1_send_data(uint8_t* data , uint16_t len);


#endif
