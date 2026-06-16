/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include "blink65.h"
#include "variant.h"
#include <peekpoke.h>


/*- LOCAL MACROS -----------------------------------------------------------*/

#define PORT(pin) variant_port[pin]
#define PR(pin) *variant_pr[PORT(pin)]
#define DDR(pin) *variant_ddr[PORT(pin)]


/*- GLOBAL VARIABLES -------------------------------------------------------*/

uint32_t variant_clock_hz;
uint16_t variant_system_isr;
volatile uint32_t variant_time_ms;


/*- LOCAL FUNCTIONS --------------------------------------------------------*/

static uint32_t clock_ms()
{
    uint32_t time;
    uint32_t verify;

    do
    {
        time = variant_time_ms;
        verify = variant_time_ms;
    }
    while(time != verify);

    return time;
}

static void init(void)
{
    DBG("in\n");

    /* Store address of previous (kernal) isr */
    variant_system_isr = PEEKW(&variant_irq_vec);

    /* Point IRQ software vector to variant ISR */
    POKEW(&variant_irq_vec, (uint16_t)&variant_isr);
}


/*- GLOBAL FUNCTIONS -------------------------------------------------------*/

void main(void)
{
    noInterrupts();

    init();

    /* Variant specific initializations */
    initVariant();

    interrupts();

    /* Run the sketch */
    setup();

    for (;;)
    {
        loop();
    }
}

void delay(uint32_t milliseconds)
{
    uint32_t start = clock_ms();

    while ((clock_ms() - start) < milliseconds);
}

uint8_t digitalRead(uint8_t pin)
{
    DBG("dr pin %u\n", pin);

    if (pin >= NUM_DIGITAL_PINS)
        return LOW;

    return PR(pin) & variant_pr_mask[pin] ? HIGH : LOW;
}

void digitalWrite(uint8_t pin, uint8_t state)
{
    uint8_t mode; 

    DBG("dw pin:%u state:%u\n", pin, state);

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
    DBG("pm pin:%u mode:%u\n", pin, mode);

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

void tone(uint8_t pin, uint16_t frequency)
{
    DBG("tf pin:%u hz:%u\n", pin, frequency);

    if (frequency < 10)
        frequency = 10;

    tonePeriod(pin, (variant_clock_hz + frequency) / (frequency * 2));
}

/*--------------------------------------------------------------------------*/
