/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "blink65.h"
#include <time.h>
#include "variant.h"

#ifdef DEBUG
#include <stdio.h>
#endif

/*- LOCAL MACROS -----------------------------------------------------------*/

#ifndef DEBUG
#define DBG(...) 
#else
#define DBG(format, ...) printf("[D] " format "\n", __VA_ARGS__)
#endif

#define PORT(pin) variant_port[pin]
#define PR(pin) *variant_pr[PORT(pin)]
#define DDR(pin) *variant_ddr[PORT(pin)]

/*- LOCAL FUNCTIONS --------------------------------------------------------*/

static void init(void)
{
    /* ... */
}


/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void main(void)
{
    init();

    /* Variant specific initializations */
    initVariant();

    /* Run the sketch */
    setup();

    for (;;)
    {
        loop();
    }
}

void delay(uint32_t milliseconds)
{
    clock_t start = clock();
    clock_t end = (clock_t)milliseconds * CLOCKS_PER_SEC / 1000 + start;
    
    while (clock() < end);
}

uint8_t digitalRead(uint8_t pin)
{
    DBG("DR pin %u", pin);

    if (pin >= NUM_DIGITAL_PINS)
        return LOW;

    return PR(pin) & variant_pr_mask[pin] ? HIGH : LOW;
}

void digitalWrite(uint8_t pin, uint8_t state)
{
    uint8_t mode; 

    DBG("DW pin:%u state:%u", pin, state);

    if (pin >= NUM_DIGITAL_PINS)
        return;

    mode = DDR(pin) & variant_ddr_mask[pin] ? OUTPUT : INPUT;

    if (state == LOW)
        PR(pin) &= ~variant_pr_mask[pin];
    else
        PR(pin) |= variant_pr_mask[pin];

    if (pin == LED_BUILTIN)
        updateBuiltinLed(mode, state);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    DBG("PM pin:%u mode:%u", pin, mode);

    if (pin >= NUM_DIGITAL_PINS)
        return;

    if (mode == INPUT)
        DDR(pin) &= ~variant_ddr_mask[pin];
    else if (mode == OUTPUT)
        DDR(pin) |= variant_ddr_mask[pin];

    if (pin == LED_BUILTIN)
    {
        uint8_t state = digitalRead(LED_BUILTIN);

        updateBuiltinLed(mode, state);
    }
}

/*--------------------------------------------------------------------------*/
