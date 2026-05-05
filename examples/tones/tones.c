/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include <blink65.h>

#if defined(__C64__)
#define SECOND_PIN PIN_K
#elif defined(__PET__)
#define SECOND_PIN PIN_M
#elif defined(__VIC20__)
#define SECOND_PIN PIN_M
#endif

void setup(void)
{
    pinMode(PIN_L, OUTPUT);

    /* Not strictly necessary for PIN_M on PET and VIC-20 */
    pinMode(SECOND_PIN, OUTPUT);
}

void loop(void)
{
    tone(PIN_L, 20);
    tone(SECOND_PIN, 20);
    delay(5000);

    tone(PIN_L, 200);
    tone(SECOND_PIN, 200);
    delay(5000);

    tone(PIN_L, 2000);
    tone(SECOND_PIN, 2000);
    delay(5000);

    tone(PIN_L, 20000);
    tone(SECOND_PIN, 20000);
    delay(5000);

    noTone(PIN_L);
    noTone(SECOND_PIN);
    delay(5000);
}

/*--------------------------------------------------------------------------*/
