/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#ifndef VARIANT_H 
#define VARIANT_H

#include <stdint.h>

/*- CONSTANTS --------------------------------------------------------------*/

#define COLOR_INPUT COLOR_YELLOW
#define COLOR_LOW   COLOR_BLACK
#define COLOR_HIGH  COLOR_RED

/*- VARIABLES --------------------------------------------------------------*/

/* Port Register */
extern uint8_t * _pr;

/* Data Direction Register */
extern uint8_t * _ddr;

/*- FUNCTIONS --------------------------------------------------------------*/

/**
 * Variant specific initialization.
 */
void initVariant(void);

/**
 * Update GUI to replicate the satuts of the fake builtin LED.
 */
void updateBuiltinLed(
	uint8_t mode, /**< builtin LED mode, INPUT or PUTPUT */
	uint8_t state /**< builtin LED state, LOW or HIGH */
);

#endif /* VARIANT_H */
/*--------------------------------------------------------------------------*/
