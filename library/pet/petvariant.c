/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <peekpoke.h>
#include <pet.h>
#include "blink65.h"

/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void initVariant(void)
{
    _pr = &VIA.pra;
    _ddr = &VIA.ddra;
}

void updateBuiltinLed(uint8_t mode, uint8_t state)
{
    if (mode == INPUT || state == LOW)
        POKE(0x8000, PEEK(0x8000) & 0x7F);
    else
        POKE(0x8000, PEEK(0x8000) | 0x80);
}

/*--------------------------------------------------------------------------*/
