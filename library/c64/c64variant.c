/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <c64.h>
#include "blink65.h"


/*- GLOBAL VARIABLES -------------------------------------------------------*/

/* Pin to port LUT */
uint8_t variant_port[NUM_DIGITAL_PINS] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    1
};

uint8_t * variant_pr[NUM_DIGITAL_PORTS] =
{
	&CIA2.prb,
	&CIA2.pra
};

uint8_t variant_pr_mask[NUM_DIGITAL_PINS] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x04
};

uint8_t * variant_ddr[NUM_DIGITAL_PORTS] =
{
    &CIA2.ddrb,
    &CIA2.ddra
};

uint8_t variant_ddr_mask[NUM_DIGITAL_PINS] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x04
};


/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void initVariant(void)
{
    /* ... */
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
