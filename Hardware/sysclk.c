#include "sysclk.h"

void clock_initialize(void)
{
    sysclk_initialize();
    SysTick_initialize();
}

void sysclk_initialize(void)
{
    RCM_Reset();
    RCM_ConfigHSE(RCM_HSE_OPEN);

    if (RCM_WaitHSEReady() == SUCCESS)
    {
        FMC_EnablePrefetchBuffer();
        FMC_ConfigLatency(FMC_LATENCY_2);

        RCM_ConfigAHB(RCM_AHB_DIV_1);
        RCM_ConfigAPB2(RCM_APB_DIV_1);
        RCM_ConfigAPB1(RCM_APB_DIV_2);

        RCM_ConfigPLL(RCM_PLLSEL_HSE, RCM_PLLMF_15);
        RCM_EnablePLL();
        while (RCM_ReadStatusFlag(RCM_FLAG_PLLRDY) == RESET);

        RCM_ConfigSYSCLK(RCM_SYSCLK_SEL_PLL);
        while (RCM_ReadSYSCLKSource() != RCM_SYSCLK_SEL_PLL);

        RCM_EnableCSS();
    }
    else
    {
        while (1);
    }
}

void SysTick_initialize(void)
{
    SysTick_ConfigCLKSource(SYSTICK_CLK_SOURCE_HCLK_DIV8);
}

void delay_us(uint32_t nus)
{
    uint32_t temp;

	SysTick->LOAD = (uint32_t)(nus * 120 / 8);
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x01;

	do
	{
		temp = SysTick->CTRL;
	} while (!((temp) >> 4) & 0x00000001);
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0X00;
}

void delay_ms(uint32_t nms)
{
    while(nms--)
    {
        delay_us(1000);
    }
}


