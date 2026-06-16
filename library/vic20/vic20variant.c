/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <peekpoke.h>
#include <vic20.h>
#include "blink65.h"


/*- CONSTATNTS -------------------------------------------------------------*/

#define CLOCK_PAL  1108404
#define CLOCK_NTSC 1022727

#define ACR_T1_MASK       0xC0
#define ACR_T1_OUT_ENABLE 0x80 /* Output pulses or square-wave on PB7 */
#define ACR_T1_FREE_RUN   0x40 /* One-shot when not free-run */
#define ACR_SR_MASK       0x1C
#define ACR_SR_FREE_RUN   0x10 /* Shift out free run T2 rate */

#define IXR_ENABLE 0x80
#define IXR_TIMER1 0x40

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
    if (*(int8_t *)0xE475 < 0)
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

    /* VIA 2 timer 1 is already configured to continuously generate
     * interrupts at 60 Hz, change period to count milliseconds */
    POKEW(&VIA2.t1_lo, (variant_clock_hz / 1000) - 2);
}

void analogWrite(uint8_t pin, uint8_t level)
{
    uint8_t pattern = 0xFF >> (8 - level * 9 / 255);

    DBG("aw p:%u l:%3u s:$%02x\n", pin, level, pattern);

    if (pin != PIN_M)
        return;

    /* Shift register free-run */
    VIA1.acr = VIA1.acr & ~ACR_SR_MASK | ACR_SR_FREE_RUN;
    VIA1.t2_lo = 255; /* PWM frequency around 500 Hz */
    VIA1.sr = pattern;
}

void noTone(uint8_t pin)
{
    DBG("nt pin:%u\n", pin);

    if (pin == PIN_L)
    {
        /* Disable timer 1 output on PB7, switch to one-shot mode */
        VIA1.acr &= ~(ACR_T1_OUT_ENABLE | ACR_T1_FREE_RUN);    

        /* Set timer period to 0 to stop last run */
        *(uint16_t *)&VIA1.t1_lo = 0;
    }
    else if (pin == PIN_M)
    {
        /* Disable shift register */
        VIA1.acr &= ~ACR_SR_MASK;
    }
}

void tonePeriod(uint8_t pin, uint16_t period)
{
    DBG("tp pin:%u t:%u\n", pin, period);

    if (pin == PIN_L)
    {
        /* Timer 1 free-run and toggle PB7 */
        VIA1.acr |= (ACR_T1_OUT_ENABLE | ACR_T1_FREE_RUN);

        /* Set timer period and start */
        *(uint16_t *)&VIA1.t1_lo = period - 2;
    }
    else if (pin == PIN_M)
    {
        uint8_t pattern;

        /* Shift register free-run */
        VIA1.acr = VIA1.acr & ~ACR_SR_MASK | ACR_SR_FREE_RUN;

        period /= 2;
        if (period <= 257)
        {
            pattern = 0x55; /* 01010101 */
        }
        else
        {
            period /= 2;
            if (period <= 257)
            {
                pattern = 0x33; /* 00110011 */
            }
            else
            {
                period /= 2;
                if (period > 257)
                    period = 257;
             
                pattern = 0x0F; /* 00001111 */
            }
        }

        VIA1.t2_lo = (uint8_t)(period - 2);
        VIA1.sr = pattern;

        DBG("tp sr:$%02x t2:%u\n", pattern, period);
    }
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
