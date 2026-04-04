/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <c64.h>
#include "blink65.h"

/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void initVariant(void)
{
    _pr = &CIA2.prb;
    _ddr = &CIA2.ddrb;
}

void updateBuiltinLed(uint8_t mode, uint8_t state)
{
    VIC.bordercolor = mode == INPUT 
        ? COLOR_INPUT
        : state == LOW
            ? COLOR_LOW
            : COLOR_HIGH;
}

/*--------------------------------------------------------------------------*/
