#include "timer.h"

void timer_initialize(void)
{
    TMR_BaseConfig_T TMR_BaseConfigStruct;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR5);

    TMR_BaseConfigStruct.clockDivision = TMR_CLOCK_DIV_1;
    TMR_BaseConfigStruct.countMode = TMR_COUNTER_MODE_UP;
    TMR_BaseConfigStruct.division = 119;
    TMR_BaseConfigStruct.period = 999;
    TMR_BaseConfigStruct.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR5, &TMR_BaseConfigStruct);

    TMR_EnableInterrupt(TMR5, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR5_IRQn, 1, 1);

    TMR_Enable(TMR5);
}

