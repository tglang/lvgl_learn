#include "Hardware.h"

TimerCtrl_t TimerCtrl;

uint8_t i;

int main(void)
{
    Hardware_initialize();
    Software_initialize();

    while(1)
    {
        if(TimerCtrl.Flag_1ms)
        {
            TimerCtrl.Flag_1ms = 0;


        }

        if(TimerCtrl.Flag_8ms)
        {
            TimerCtrl.Flag_8ms = 0;


        }

        if(TimerCtrl.Flag_128ms)
        {
            TimerCtrl.Flag_128ms = 0;

            if(i == 0)
            {
                GPIO_SetBit(GPIOC, GPIO_PIN_12);
            }
            else
            {
                GPIO_ResetBit(GPIOC, GPIO_PIN_12);
            }
            if(++i == 2)i = 0;
        }

        if(TimerCtrl.Flag_1024ms)
        {
            TimerCtrl.Flag_1024ms = 0;


        }
    }
}


void TMR5_IRQHandler(void)
{
    if (TMR_ReadIntFlag(TMR5, TMR_INT_UPDATE) == SET)
    {
        TMR_ClearIntFlag(TMR5, TMR_INT_UPDATE);

        TimerCtrl.Counts++;

        TimerCtrl.Flag_1ms = 1;
        if((TimerCtrl.Counts & 0x000F) == 0)TimerCtrl.Flag_8ms = 1;
        if((TimerCtrl.Counts & 0x007F) == 0)TimerCtrl.Flag_128ms = 1;
        if((TimerCtrl.Counts & 0x03FF) == 0)
        {
            TimerCtrl.Flag_1024ms = 1;
            TimerCtrl.Counts = 0;
        }
    }
}
