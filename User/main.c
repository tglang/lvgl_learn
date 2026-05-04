#include "Hardware.h"

TimerCtrl_t TimerCtrl;

int main(void)
{
    Hardware_initialize();
    Software_initialize();

    //LCD_ShowString(10,35,"2.4 TFT SPI 240*320",RED);

    while(1)
    {
        if(TimerCtrl.Flag_1ms)
        {
            TimerCtrl.Flag_1ms = 0;


        }

        if(TimerCtrl.Flag_8ms)
        {
            TimerCtrl.Flag_8ms = 0;

            KeyCtrl.KeyVaule = KeyScan();
            if(KeyCtrl.KeyVaule == 1)
            {
                KeyCtrl.KeyTask = 1;
            }
            else if (KeyCtrl.KeyVaule == 2)KeyCtrl.KeyTask = 2;
            else if (KeyCtrl.KeyVaule == 3)KeyCtrl.KeyTask = 3;
            else if (KeyCtrl.KeyVaule == 4)KeyCtrl.KeyTask = 4;
        }

        if(TimerCtrl.Flag_128ms)
        {
            TimerCtrl.Flag_128ms = 0;
            if(KeyCtrl.KeyTask == 1)LED1_TOGGLE();
            if (KeyCtrl.KeyTask == 2)LED1_TOGGLE();
            else if (KeyCtrl.KeyTask == 3)LED2_TOGGLE();
            else if (KeyCtrl.KeyTask == 4)LED3_TOGGLE();
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