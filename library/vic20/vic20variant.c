/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <vic20.h>
#include "blink65.h"

/*- GLOBAL VARIABLES -------------------------------------------------------*/

/* Pin to port LUT */
uint8_t variant_port[NUM_DIGITAL_PINS] =
{
    0, 0, 0, 0, 0, 0, 0, 0, /* PB0..7 */
    1,                      /* CB2 */
    2, 2, 2, 2, 2, 2        /* PA2..7 */
};

uint8_t * variant_pr[NUM_DIGITAL_PORTS] =
{
    &VIA1.prb,
    &VIA1.pcr, /* PCR has output level of CB2  */
    &VIA1.pra
};

uint8_t variant_pr_mask[NUM_DIGITAL_PINS] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x20,
    0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

uint8_t * variant_ddr[NUM_DIGITAL_PORTS] =
{
    &VIA1.ddrb,
    &VIA1.pcr, /* PCR has direction of CB2 */
    &VIA1.ddra
};

uint8_t variant_ddr_mask[NUM_DIGITAL_PINS] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x80,
    0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};


/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void initVariant(void)
{
    /* ... */
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
