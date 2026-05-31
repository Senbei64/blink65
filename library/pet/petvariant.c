/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "variant.h"
#include <peekpoke.h>
#include <pet.h>
#include "blink65.h"


/*- CONSTATNTS -------------------------------------------------------------*/

#define CLOCK  1000000

#define ACR_SR_MASK     0x1C
#define ACR_SR_FREE_RUN 0x10 /* Shift out free run T2 rate */


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
    variant_clock_hz = 1000000;
}

void analogWrite(uint8_t pin, uint8_t level)
{
    uint8_t pattern = 0xFF >> (8 - level * 9 / 255);

    DBG("aw p:%u l:%3u s:$%02x\n", pin, level, pattern);

    if (pin != PIN_M)
        return;

    /* Shift register free-run */
    VIA.acr = VIA.acr & ~ACR_SR_MASK | ACR_SR_FREE_RUN;
    VIA.t2_lo = 255; /* PWM frequency around 500 Hz */
    VIA.sr = pattern;
}

void noTone(uint8_t pin)
{
    DBG("nt pin:%u\n", pin);

    if (pin != PIN_M)
        return;

    /* Disable shift register */
    VIA.acr &= ~ACR_SR_MASK;
}

void tonePeriod(uint8_t pin, uint16_t period)
{
    uint8_t pattern;

    DBG("tp pin:%u t:%u\n", pin, period);

    if (pin != PIN_M)
        return;

    /* Shift register free-run */
    VIA.acr = VIA.acr & ~ACR_SR_MASK | ACR_SR_FREE_RUN;

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

    VIA.t2_lo = (uint8_t)(period - 2);
    VIA.sr = pattern;

    DBG("tp sr:$%02X t2:%u\n", pattern, period - 2);
}

void updateBuiltinLed(uint8_t mode, uint8_t state)
{
    /* Reverse upper left character in video memory */
    if (mode == INPUT || state == LOW)
        POKE(0x8000, PEEK(0x8000) & 0x7F);
    else
        POKE(0x8000, PEEK(0x8000) | 0x80);
}

/*--------------------------------------------------------------------------*/
