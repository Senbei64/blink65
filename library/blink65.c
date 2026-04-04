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

/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void main(void)
{
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
    DBG("DR pin %d", pin);

    if (pin < PIN_C || pin > PIN_L)
        return LOW;

    return *_pr & (0x01 << (pin - PIN_C)) ? HIGH : LOW;
}

void digitalWrite(uint8_t pin, uint8_t state)
{
    uint8_t bit;
    uint8_t mode; 

    DBG("DW pin:%d state:%d", pin, state);

    if (pin < PIN_C || pin > PIN_L)
        return;

    bit = 0x01 << (pin - PIN_C);
    mode = *_ddr & bit ? OUTPUT : INPUT;

    if (state == LOW)
        *_pr &= ~bit;
    else
        *_pr |= bit;

    if (pin == LED_BUILTIN)
        updateBuiltinLed(mode, state);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t bit;

    DBG("PM pin:%d mode:%d", pin, mode);

    if (pin < PIN_C || pin > PIN_L)
        return;

    bit = 0x01 << (pin - PIN_C);

    if (mode == INPUT)
        *_ddr &= ~bit;
    else if (mode == OUTPUT)
        *_ddr |= bit;

    if (pin == LED_BUILTIN)
    {
        uint8_t state = digitalRead(LED_BUILTIN);

        updateBuiltinLed(mode, state);
    }
}

/*--------------------------------------------------------------------------*/
