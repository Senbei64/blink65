/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <vic20.h>
#include "blink65.h"

/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void initVariant(void)
{
    _pr = &VIA1.prb;
    _ddr = &VIA1.ddrb;
}

void updateBuiltinLed(uint8_t mode, uint8_t state)
{
    VIC.bg_border_color = VIC.bg_border_color & 0xF8 | (mode == INPUT 
        ? COLOR_INPUT
        : state == LOW
            ? COLOR_LOW
            : COLOR_HIGH);
}

/*--------------------------------------------------------------------------*/
