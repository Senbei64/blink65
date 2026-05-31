/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <c64.h>
#include "blink65.h"


/*- CONSTATNTS -------------------------------------------------------------*/

#define CLOCK_PAL   985248
#define CLOCK_NTSC 1022727

#define CRX_RUN          0x01
#define CRX_OUT_ENABLE   0x02
#define CRX_OUT_TOGGLE   0x04 /* 1 = toggle, 0 = pulse */
#define CRX_ONE_SHOT     0x08 /* 1 = one-shot, 0 = free-run */
#define CRX_LOAD         0x10
#define CRA_CLOCK        0x20 /* 1 = CNT, 0 = system */
#define CRB_CLOCK_MASK   0x60 /* 00 = system */

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
    if (*(uint8_t *)0x02A6 != 0)
    {
        /* PAL */
        DBG("iv pal\n");
        variant_clock_hz = CLOCK_PAL;
    }
    else
    {
        /* NTSC */
        DBG("iv ntsc\n");
        variant_clock_hz = CLOCK_NTSC;
    }

    /* Timer A and B of CIA 2 use system clock */
    CIA2.cra &= ~CRA_CLOCK;
    CIA2.crb &= ~CRB_CLOCK_MASK;
}

void noTone(uint8_t pin)
{
    DBG("nt pin:%u\n", pin);

    if (pin != PIN_K && pin != PIN_L)
        return;

    if (pin == PIN_K)
        CIA2.cra &= ~CRX_RUN;
    else
        CIA2.crb &= ~CRX_RUN;
}

void tonePeriod(uint8_t pin, uint16_t period)
{
    DBG("tp pin:%u t:%u\n", pin, period);

    if (pin != PIN_K && pin != PIN_L)
        return;

    if (pin == PIN_K)
    {
        /* Set timer period */
        *(uint16_t *)&CIA2.ta_lo = period - 1;

        /* Timer A free-run and toggle PB6 */
        CIA2.cra = CIA2.cra
                 & ~CRX_ONE_SHOT
                 | (CRX_RUN | CRX_OUT_ENABLE | CRX_OUT_TOGGLE | CRX_LOAD);
    }
    else
    {
        /* Set timer period */
        *(uint16_t *)&CIA2.tb_lo = period - 1;

        /* Timer B free-run and toggle PB7 */
        CIA2.crb = CIA2.crb
                 & ~CRX_ONE_SHOT
                 | (CRX_RUN | CRX_OUT_ENABLE | CRX_OUT_TOGGLE | CRX_LOAD);
    }
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
