/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#ifdef __CC65__
#include <blink65.h>
#endif

#define RECEIVE_PIN 0 /* IR receiver (VS1838) */
#define SERVE_PIN 1 /* SW1 push button */
#define FIRST_LED 2
#define LED_COUNT 5 /* Number of LEDs in the ball sequence, FIRST_LED onward */

/* Pin for IR emitter must be tone capable in blink65 */
#define IR_LED_PIN 7

static uint8_t count;

void setup(void)
{
    for (count = FIRST_LED; count < FIRST_LED + LED_COUNT; ++count) {
        pinMode(count, OUTPUT);
        digitalWrite(count, LOW);
    }

    pinMode(IR_LED_PIN, OUTPUT);

    /* Plain INPUT: the pull-up resistor is provided by the external circuit */
    pinMode(RECEIVE_PIN, INPUT);
    pinMode(SERVE_PIN, INPUT);
}

void loop(void)
{
    for (count = 0; count == 0;) {
        if (digitalRead(RECEIVE_PIN) == LOW)
            count = FIRST_LED;
        else if (digitalRead(SERVE_PIN) == LOW)
            count = FIRST_LED + LED_COUNT / 2;
    }

    /* Debounce pause */
    delay(150);

    /* Scroll the LEDs across the field: the ball travels towards the launch */
    for (; count < FIRST_LED + LED_COUNT; ++count) {
        digitalWrite(count, HIGH);
        delay(200);
        digitalWrite(count, LOW);
    }

    /* Launch: drive the IR LED at 38kHz so the opponent can receive it */
    for (count = 0; count < 15; ++count) {
        tone(IR_LED_PIN, 38000u);
        delay(10);
        noTone(IR_LED_PIN);
        delay(10);
    }

    /* Protective pause: gives time to release the button and avoids this
     * circuit reading its own IR reflections off nearby surfaces */
    delay(800);
}

/*--------------------------------------------------------------------------*/
