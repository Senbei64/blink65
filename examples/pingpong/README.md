# blink65
Blink65 is a project created and maintained by Fabio Carignano.

Write Arduino-style sketches with cc65 for VIC-20, PET, and Commodore 64.

## Ping Pong
Two identical copies of this circuit and sketch play table tennis against each other, passing the ball back and forth with infrared light.
Players can be any mix of Commodore computers and real Arduino boards: a VIC-20 can play against a Commodore 64 or an Arduino.

Each copy waits for the ball to arrive, either from the opponent through the VS1838 IR
receiver or from the serve push button, then scrolls a sequence of LEDs across the
field and fires a 38kHz burst on the IR LED using `tone()` to send the ball back.

### Pins
Pins are given as plain numbers, so the same sketch can be copied and pasted into the Arduino IDE.
On the Commodore user port they correspond to:

| Pin | blink65 | Function |
|-----|---------|----------|
| 0 | `PIN_C` | VS1838 IR receiver output |
| 1 | `PIN_D` | Serve push button |
| 2-6 | `PIN_E` - `PIN_K` | Ball LEDs |
| 7 | `PIN_L` | IR LED, must be tone capable |

Receiver and push button have external pull-up resistors, so their pins are plain `INPUT`.

### Targets
Only Commodore 64 and VIC-20 are supported: on the PET pin 7 (`PIN_L`) cannot generate a tone.
The PET could drive the IR LED from its tone capable pin 8 (`PIN_M`) instead,
as shown in the [IR Loop](../irloop/README.md) example.

### Arduino UNO Q
On the Arduino UNO Q `tone()` cannot be used: due to the limits of its Zephyr based core,
it cannot reach the 38kHz of the IR carrier.
The board is fast enough, though, to generate the carrier by bit banging the IR LED pin.
Replace each `tone()`, `delay(10)`, `noTone()` sequence with a call to a function like this one:

```c
#define TONE38_CYCLES 380 /* trimmed for ~38kHz, gives ~10ms burst */

void tone38(void)
{
    for (unsigned int i = 0; i < TONE38_CYCLES; ++i) {
        digitalWrite(IR_LED_PIN, HIGH);
        delayMicroseconds((i & 1) ? 13 : 14);
        digitalWrite(IR_LED_PIN, LOW);
        delayMicroseconds(13);
    }
}
```

### Ping pong IR between VIC-20 and Arduino duemilanove

https://github.com/user-attachments/assets/1afbe075-2fae-4645-a4b7-778dc142d86a

### Ping pong IR between C64U and Arduino UNO Q

https://github.com/user-attachments/assets/3cfacd35-acb2-4ebf-9360-9ccf1e7f0e60
