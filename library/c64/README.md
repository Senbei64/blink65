# blink65
Blink65 is a project created and maintained by Fabio Carignano.

Write Arduino-style sketches with cc65 for VIC-20, PET, and Commodore 64.

### Commodore 64 User Port

|  PIN_1  |  PIN_2  |  PIN_3  |  PIN_4  |  PIN_5  |  PIN_6  |  PIN_7  |  PIN_8  |  PIN_9  |  PIN_10 |  PIN_11 |  PIN_12 |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
|   GND   |  +5V DC |  /RST   |  _(1)_  |   SP1~  |  _(1)_  |   SP2~  |  _(1)_  |  _(1)_  | 9V AC + | 9V AC - |   GND   |
|   GND   |  _(1)_  |   PB0   |   PB1   |   PB2   |   PB3   |   PB4   |   PB5   |   PB6~  |   PB7~  |   PA2   |   GND   |
|**PIN_A**|**PIN_B**|**PIN_C**|**PIN_D**|**PIN_E**|**PIN_F**|**PIN_H**|**PIN_J**|**PIN_K**|**PIN_L**|**PIN_M**|**PIN_N**|

_(1)_ Pins available via direct programming of CIA chips.

### Commodore 64 Variant

The Commodore 64 features two 6526 CIA chips.
The User Port exposes I/O pins from the second CIA, and Serial Port pins from both.

The ```tone()``` function uses Timer A and Timer B of CIA 2 in free-run mode to toggle pins PB6 (**PIN_K**) and PB7 (**PIN_L**) respectively at the requested frequency.

The supported frequency range is between 10 Hz and 65 kHz.

Pins PB6 (**PIN_K**) and PB7 (**PIN_L**) must be explicitly set to output mode when using ```tone()```.

The ```analogWrite()``` function uses Timer B of CIA 1 to generate an interrupt for the base 1 kHz PWM frequency (which is also used to increment the milliseconds counter).
It then implements different output methods depending on the pin:
- PB6 (**PIN_K**): Uses Timer A of CIA 1 in one-shot mode to generate a single pulse on pin PB6.
- PB7 (**PIN_L**): Uses Timer B of CIA 1 in one-shot mode to generate a single pulse on pin PB7.
- SP1 (**PIN_5**): Uses Timer A of CIA 1 along with the Serial Port in one-shot mode to output patterns that generate a single pulse.
- SP2 (**PIN_7**): Uses Timer A of CIA 2 along with the Serial Port in one-shot mode to output patterns that generate a single pulse.

Note that since both PIN_K and PIN_5 require Timer A of CIA 1 to implement PWM,
they cannot be used simultaneously.
You can, however, use PIN_5 as an analog output while using PIN_K as digital input or output.