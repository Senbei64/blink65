/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <peekpoke.h>
#include <pet.h>
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
    &VIA.prb,
    &VIA.pcr, /* PCR has output level of CA2 and CB2  */
};

uint8_t variant_pr_mask[NUM_DIGITAL_PINS] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x20
};

uint8_t * variant_ddr[NUM_DIGITAL_PORTS] =
{
    &VIA.ddrb,
    &VIA.pcr /* PCR has direction of CA2 and CB2 */
};

uint8_t variant_ddr_mask[NUM_DIGITAL_PINS] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x80
};


/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void initVariant(void)
{
    /* ... */
}

void updateBuiltinLed(uint8_t mode, uint8_t state)
{
    if (mode == INPUT || state == LOW)
        POKE(0x8000, PEEK(0x8000) & 0x7F);
    else
        POKE(0x8000, PEEK(0x8000) | 0x80);
}

/*--------------------------------------------------------------------------*/
