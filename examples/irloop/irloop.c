/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include <blink65.h>

#define TICK_MS   10
#define ON_TICKS  (5000 / TICK_MS)
#define OFF_TICKS (5000 / TICK_MS)

#define STATE_ON  0
#define STATE_OFF 1

#define RECEIVER_PIN PIN_D /* IR receiver (VS1838) */

/* Pin for IR emitter must be tone capable */
#if defined(__C64__)
#define IR_LED_PIN PIN_L
#elif defined(__PET__)
#define IR_LED_PIN PIN_M
#elif defined(__VIC20__)
#define IR_LED_PIN PIN_L
#endif

static uint8_t state;
static uint16_t tick_count;

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(IR_LED_PIN, OUTPUT);
    pinMode(RECEIVER_PIN, INPUT);

    state = STATE_ON;
    tick_count = 0;
}

void loop(void)
{
    delay(TICK_MS);
    ++tick_count;

    /* Show the status of the IR receiver with the default LED */
    digitalWrite(LED_BUILTIN, digitalRead(RECEIVER_PIN));

    switch (state) {
    case STATE_ON:
        /* 10 ms bursts of 38 kHz carrier */

        /* Time to switch state? */
        if (tick_count >= ON_TICKS) {
            noTone(IR_LED_PIN);
            state = STATE_OFF;
            tick_count = 0;
        } else {
            if (tick_count & 1)
                tone(IR_LED_PIN, 38000u);
            else
                noTone(IR_LED_PIN);
        }
        break;

    case STATE_OFF:
        /* IR emitter silent */

        /* Time to switch state? */
        if (tick_count >= OFF_TICKS) {
            tone(IR_LED_PIN, 38000u);
            state = STATE_ON;
            tick_count = 0;
        }
        break;
    }
}

/*--------------------------------------------------------------------------*/
