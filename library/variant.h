/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#ifndef VARIANT_H 
#define VARIANT_H

#include <stdint.h>


/*- CONSTANTS --------------------------------------------------------------*/

#define COLOR_INPUT COLOR_YELLOW
#define COLOR_LOW   COLOR_BLACK
#define COLOR_HIGH  COLOR_RED


/*- VARIABLES --------------------------------------------------------------*/

/* System clock speed in hertz */
extern uint32_t variant_clock_hz;

/* Pin to port LUT */
extern uint8_t variant_port[];

/* Port to Port Register LUT */
extern uint8_t * variant_pr[];

extern uint8_t variant_pr_mask[];

/* Port to Data Direction Register LUT */
extern uint8_t * variant_ddr[];

extern uint8_t variant_ddr_mask[];


/*- FUNCTIONS --------------------------------------------------------------*/

/**
 * Variant specific initialization.
 */
void initVariant(void);

/**
 * Output a square wave with the given period.
 */
void tonePeriod(
    uint8_t  pin,   /**< pin number */
    uint16_t period /**< period in clock cycles */
);

/**
 * Update GUI to replicate the satuts of the fake builtin LED.
 */
void updateBuiltinLed(
	uint8_t mode, /**< builtin LED mode, INPUT or PUTPUT */
	uint8_t state /**< builtin LED state, LOW or HIGH */
);

#endif /* VARIANT_H */
/*--------------------------------------------------------------------------*/
