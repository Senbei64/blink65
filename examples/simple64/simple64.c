/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include <blink65.h>

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void)
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}

/*--------------------------------------------------------------------------*/
