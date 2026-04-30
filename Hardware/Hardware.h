#ifndef __Hardware_H__
#define __Hardware_H__

#include "string.h"

#include "apm32e10x.h"
#include "apm32e10x_rcm.h"
#include "apm32e10x_misc.h"
#include "apm32e10x_tmr.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_usart.h"
#include "apm32e10x_dma.h"



#include "gpio.h"
#include "usart.h"
#include "timer.h"


#include "Software.h"
#include "Data.h"

void Hardware_initialize(void);


#endif
