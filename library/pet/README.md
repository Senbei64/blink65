# blink65
Blink65 is a project created and maintained by Fabio Carignano.

Write Arduino-style sketches with cc65 for VIC-20, PET, and Commodore 64.

### Commodore PET User Port

|  PIN_1  |  PIN_2  |  PIN_3  |  PIN_4  |  PIN_5  |  PIN_6  |  PIN_7  |  PIN_8  |  PIN_9  |  PIN_10 |  PIN_11 |  PIN_12 |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
|   GND   |  _(1)_  |  _(2)_  |  _(2)_  |  _(2)_  |  _(2)_  |  _(2)_  |  _(2)_  |  _(1)_  |  _(1)_  |   GND   |   GND   |
|   GND   |  _(2)_  |   PA0   |   PA1   |   PA2   |   PA3   |   PA4   |   PA5   |   PA6   |   PA7   |   CB2~  |   GND   |
|**PIN_A**|**PIN_B**|**PIN_C**|**PIN_D**|**PIN_E**|**PIN_F**|**PIN_H**|**PIN_J**|**PIN_K**|**PIN_L**|**PIN_M**|**PIN_N**|

_(1)_ Pins used for the video signal.

_(2)_ Pins available via direct programming of PIA and VIA chips.

### Commodore PET Variant

The Commodore PET line of computers all feature two 6520 PIA chips and one 6522 VIA chip.
Blink65 utilizes only the VIA chip.

The two timers are allocated as follows:
- VIA Timer 1: 1 kHz interrupt to increment the system clock and trigger high-resolution PWM pulses
- VIA Timer 2: limited-range tone on PIN_M, high-resolution PWM on PIN_M

The ```tone()``` function uses the VIA Shift Register in free-run mode,
combining the Timer 2 rate with specific Shift Register patterns to output the required frequency on pin CB2 (**PIN_M**).

The supported frequency range with this method is between 243 Hz and 65 kHz.

Pin CB2 (**PIN_M**) is implicitly set to output mode when using ```tone()```.

The ```analogWrite()``` function uses Timer 1 of the VIA to generate an interrupt for the base 1 kHz PWM frequency (which is also used to increment the milliseconds counter).
It then uses Timer 2 along with the Shift Register in one-shot mode to output patterns on pin CB2 (**PIN_M**) that generate a single pulse.
