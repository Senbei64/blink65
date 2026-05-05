/*- blink65 - Copyright 2026 Fabio Carignano -------------------------------*/

#include <blink65.h>

static uint8_t level;

void setup(void)
{
    level = 0;
}

void loop(void)
{
    analogWrite(PIN_M, level);
    level += 10; /* Let level wrap automatically */
    delay(500);
}

/*--------------------------------------------------------------------------*/
