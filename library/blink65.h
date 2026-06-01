/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#ifndef BLINK65_H 
#define BLINK65_H

#include <stdint.h>

#if defined(__C64__)
#include <c64/pins.h>
#elif defined(__PET__)
#include <pet/pins.h>
#elif defined(__VIC20__)
#include <vic20/pins.h>
#endif

#ifdef DEBUG
#include <stdio.h>
#endif


/*- CONSTANTS --------------------------------------------------------------*/

#define INPUT  0 /**< Pin mode input, high Z */
#define OUTPUT 1 /**< Pin mode output */

#define LOW  0 /**< Output state low */
#define HIGH 1 /**< Output state high */


/*- MACROS -----------------------------------------------------------------*/

/** 
 * Print a formatted message to the console in DEBUG mode.
 */
#ifndef DEBUG
#define DBG(...) 
#else
#define DBG(...) printf("!" __VA_ARGS__)
#endif

/**
 * Enables maskable interrupts.
 */
#define interrupts() __asm__("cli")

/**
 * Disables maskable interrupts.
 */
#define noInterrupts() __asm__("sei")


/*- EXPORTED LIBRARY FUNCTIONS ---------------------------------------------*/

/**
 * Stops execution for the given amount of milliseconds.
 */
void delay(
    uint32_t milliseconds /**< delay in milliseconds */
);

/**
 * Sets the output level for an analog pin using Pulse Width Modulation (PWM).
 * PWM frequency and resolution may vary depending on the hardware variant and the
 * specific pin used.
 */
void analogWrite(
    uint8_t pin,  /**< pin number */
    uint8_t level /**< output level between 0 and 255 (Vcc) */
);

/**
 * Reads state of an input pin.
 */
uint8_t digitalRead(
    uint8_t pin /**< pin number */
);

/**
 * Sets state of an output pin.
 */
void digitalWrite(
    uint8_t pin,  /**< pin number */
    uint8_t state /**< new state, either LOW or HIGH */
);

/**
 * Stops tone output.
 */
void noTone(
    uint8_t pin /**< pin number */
);

/**
 * Sets mode of a pin.
 */
void pinMode(
    uint8_t pin, /**< pin number */
    uint8_t mode /**< new mode, either INPUT or OUTPUT */
);

/**
 * Outputs a square wave with the given frequency in the range 10Hz..65kHz.
 */
void tone(
    uint8_t  pin,      /**< pin number */
    uint16_t frequency /**< frequency im hertz */
);


/*- IMPORTED USER FUNCTIONS ------------------------------------------------*/

/**
 * Called once before loop().
 */
void setup(void);

/**
 * Called repeatedly after setup().
 */
void loop(void);

#endif /* BLINK65_H */
/*--------------------------------------------------------------------------*/
