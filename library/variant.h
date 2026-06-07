/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#ifndef VARIANT_H 
#define VARIANT_H

#include <stdint.h>


/*- CONSTANTS --------------------------------------------------------------*/

#define COLOR_INPUT COLOR_YELLOW /**< Simulated default LED in input mode */
#define COLOR_LOW   COLOR_BLACK  /**< Simulated default LED in low output */
#define COLOR_HIGH  COLOR_RED    /**< Simulated default LED in high output */


/*- VARIABLES --------------------------------------------------------------*/

/** System clock speed in hertz */
extern uint32_t variant_clock_hz;

/** Pointer to system interrupt service routine. */
extern uint16_t * variant_irq_vec;

/** Elapsed time in milliseconds */
extern volatile uint32_t variant_time_ms;

/** Pin to port LUT */
extern uint8_t variant_port[];

/** Port to Port Register LUT */
extern uint8_t * variant_pr[];

/** Pin bits in Port Register */
extern uint8_t variant_pr_mask[];

/** Port to Data Direction Register LUT */
extern uint8_t * variant_ddr[];

/** Pin bits in Data Direction Register */
extern uint8_t variant_ddr_mask[];


/*- FUNCTIONS --------------------------------------------------------------*/

/**
 * Variant-specific initialization.
 */
void initVariant(void);

/**
 * Outputs a square wave with the given period.
 */
void tonePeriod(
    uint8_t  pin,   /**< pin number */
    uint16_t period /**< period in clock cycles */
);

/**
 * Updates GUI to replicate the status of the fake built-in LED.
 */
void updateBuiltinLed(
	uint8_t mode, /**< built-in LED mode, INPUT or OUTPUT */
	uint8_t state /**< built-in LED state, LOW or HIGH */
);

/**
 * Variant-specific main interrupt service routine.
 * 
 * Updates the millisecond counter.
 */
void variant_isr(void);

#endif /* VARIANT_H */
/*--------------------------------------------------------------------------*/
