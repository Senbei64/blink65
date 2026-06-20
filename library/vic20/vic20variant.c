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
#define ACR_SR_FREE_RUN   0x10 /* Shift out free-run T2 rate */
#define ACR_SR_ONE_SHOT   0x14 /* Shift out one-shot T2 rate */

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


/*- VARIANT VARIABLES ------------------------------------------------------*/

/* Used in the ISR for PWN on pin L */
uint16_t vic20_pwm_width = 0;

/* Used in the ISR for PWN on pin M */
uint8_t vic20_pwm_pattern = 0x8000;


/*- LOCAL VARIABLES --------------------------------------------------------*/

/* Initialzed in initVariant(), used in analogWrite() */
static uint16_t timer1_period;

static const uint8_t shift_register_pattern[] =
{
    0x80, /* 1000 0000 (0...) */
    0xC0, /* 1100 0000 (0...) */
    0xE0, /* 1110 0000 (0...) */
    0xF0, /* 1111 0000 (0...) */
    0x0F, /* 0000 1111 (1...) */
    0x1F, /* 0001 1111 (1...) */
    0x3F, /* 0011 1111 (1...) */
    0x7F  /* 0111 1111 (1...) */
};


/*- LOCAL FUNCTIONS --------------------------------------------------------*/

#ifdef DEBUG
static char * patternToString(uint8_t pattern)
{
    static char str[8];
    uint8_t i;
    uint8_t bitMask;

    for (i = 0, bitMask = 0x80; bitMask; ++i, bitMask >>= 1)
        str[i] = (pattern & bitMask) ? '1' : '0';

    return str;
}
#endif


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

    timer1_period = variant_clock_hz / 1000;

    /* VIA 2 timer 1 is already configured to continuously generate
     * interrupts at 60 Hz, change period to count milliseconds */
    POKEW(&VIA2.t1_lo, timer1_period - 2);
}

#if 0
/* This version can output PWM at 9 levels, little more then 3 bit resolution */
void analogWriteLowRes(uint8_t pin, uint8_t level)
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
#endif

void analogWrite(uint8_t pin, uint8_t level)
{
    DBG("aw p:%u l:%3u\n", pin, level);

    if (pin == PIN_L)
    {
        if (level == 0)
        {
            /* Disable PWM and output a steady LOW digital level to free timer 1 */
            vic20_pwm_width = 0x8000;
            VIA1.acr &= ~ACR_T1_OUT_ENABLE;
            digitalWrite(pin, LOW);
        }
        else if (level == 255)
        {
            /* Disable PWM and output a steady HIGH digital level to free timer 1 */
            vic20_pwm_width = 0x8000;
            VIA1.acr &= ~ACR_T1_OUT_ENABLE;
            digitalWrite(pin, HIGH);
        }
        else
        {
            /* Timer 1 one-shot produces negative pulses, invert level */
            vic20_pwm_width = (uint32_t)timer1_period * (255 - level) / 255 - 2;

            DBG("aw per:%3u\n", vic20_pwm_width);

            /* Prepare timer 1 one-shots from ISR */
            VIA1.acr = VIA1.acr & ~ACR_T1_FREE_RUN | ACR_T1_OUT_ENABLE ;
        }
    }
    else if (pin == PIN_M)
    {
        if (level == 0)
        {
            /* Disable PWM and output a steady LOW digital level to free timer 2 */
            vic20_pwm_pattern = 0;
            digitalWrite(pin, LOW);
        }
        else if (level == 255)
        {
            /* Disable PWM and output a steady HIGH digital level to free timer 2 */
            vic20_pwm_pattern = 0;
            digitalWrite(pin, HIGH);
        }
        else
        {
            uint8_t tier = level >> 5;
            uint8_t divisor;
            uint8_t period;

            vic20_pwm_pattern = shift_register_pattern[tier];

            if (tier <= 3)
            {
                divisor = tier + 1;
            }
            else
            {
                divisor = 8 - tier;
                level = 255 - level;
            }

            /* The further division by 2 is necessary because, when controlled
             * by Timer 2, the maximum bit rate is Phi2 / 4. We could
             * implement an additional tier where the bit rate is controlled
             * by Phi2 directly to reach half the Phi2 rate, but this is not
             * needed for an 8-bit resolution and a base period of around 1000
             * clocks */
            period = (uint32_t)timer1_period * level / 255 / divisor / 2;

            DBG("aw t%ud%up%3u %s\n", tier, divisor, period, patternToString(vic20_pwm_pattern));

            /* Prepare shift register one-shots from ISR */
            VIA1.acr = VIA1.acr & ~ACR_SR_MASK | ACR_SR_ONE_SHOT;
            VIA1.t2_lo = period - 2;
        }
    }
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
