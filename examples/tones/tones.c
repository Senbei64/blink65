/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include <blink65.h>

#if defined(__C64__)
#define FIRST_PIN  PIN_K
#define SECOND_PIN PIN_L
#elif defined(__PET__)
#define FIRST_PIN  PIN_L
#define SECOND_PIN PIN_M
#elif defined(__VIC20__)
#define FIRST_PIN  PIN_M
#define SECOND_PIN PIN_M
#endif

void setup(void)
{
    /* Not strictly necessary for PIN_M on PET and VIC-20 */
    pinMode(FIRST_PIN, OUTPUT);
    pinMode(SECOND_PIN, OUTPUT);
}

void loop(void)
{
    /* Raised to around 250 Hz PIN_M of PET and VIC-20 */
    tone(FIRST_PIN, 20);
    tone(SECOND_PIN, 20);
    delay(5000);

    /* Raised to around 250 Hz PIN_M of PET and VIC-20 */
    tone(FIRST_PIN, 200);
    tone(SECOND_PIN, 200);
    delay(5000);

    tone(FIRST_PIN, 2000);
    tone(SECOND_PIN, 2000);
    delay(5000);

    tone(FIRST_PIN, 20000);
    tone(SECOND_PIN, 20000);
    delay(5000);

    noTone(FIRST_PIN);
    noTone(SECOND_PIN);
    delay(5000);
}

/*--------------------------------------------------------------------------*/
