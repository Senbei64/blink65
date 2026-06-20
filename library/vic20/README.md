# blink65
Blink65 is a project created and maintained by Fabio Carignano.

Write Arduino-style sketches with cc65 for VIC-20, PET, and Commodore 64.

### Commodore VIC-20 User Port

|  PIN_1  |  PIN_2  |  PIN_3  |  PIN_4  |  PIN_5  |  PIN_6  |  PIN_7  |  PIN_8  |  PIN_9  |  PIN_10 |  PIN_11 |  PIN_12 |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
|   GND   |  +5V DC |  /RST   |   PA2   |   PA3   |   PA4   |   PA5   |   PA6   |   PA7   | 9V AC + | 9V AC - |   GND   |
|   GND   |  _(1)_  |   PB0   |   PB1   |   PB2   |   PB3   |   PB4   |   PB5   |   PB6   |   PB7~  |   CB2~  |   GND   |
|**PIN_A**|**PIN_B**|**PIN_C**|**PIN_D**|**PIN_E**|**PIN_F**|**PIN_H**|**PIN_J**|**PIN_K**|**PIN_L**|**PIN_M**|**PIN_N**|

_(1)_ Pins available via direct programming of VIA chips.

### Commodore VIC-20 Variant

The Commodore VIC-20 features two 6522 VIA chips.
The User Port exposes the pins of the first VIA.

Blink65 utilizes resources from both chips to implement its functions.
Specifically, the four timers are allocated as follows:
- VIA 1 Timer 1: wide-range tone on PIN_L, high-resolution PWM on PIN_L
- VIA 1 Timer 2: limited-range tone on PIN_M, high-resolution PWM on PIN_M
- VIA 2 Timer 1: 1 kHz interrupt to increment the system clock and trigger high-resolution PWM pulses
- VIA 2 Timer 2: unused

The ```tone()``` function implements two different output methods depending on the selected pin:
- PB7 (**PIN_L**): Uses Timer 1 of the first 6522 VIA in free-run mode to toggle the pin.
  
  This 16-bit timer allows a very wide frequency range, which is clamped between 10 Hz and 65 kHz in this implementation.

  The pin must be explicitly set to output mode.
- CB2 (**PIN_M**): Uses the VIA 1 Shift Register in free-run mode,
  combining the Timer 2 rate with specific Shift Register patterns to output the requested frequency.

  The supported frequency range for this method is between 271 Hz and 65 kHz on PAL systems,
  and between 248 Hz and 65 kHz on NTSC systems.

  When using this method, PIN_M is implicitly set to output mode, as is PIN_B,
  where Timer 2 clock is emitted.

The ```analogWrite()``` function uses Timer 1 of VIA 2 to generate an interrupt for the base 1 kHz PWM frequency (which is also used to increment the milliseconds counter).
It then implements two different output methods depending on the pin:
- PB7 (**PIN_L**): Uses Timer 1 of VIA 1 in one-shot mode to generate a single pulse on pin PB7.

  The pin must be explicitly set to output mode.
- CB2 (**PIN_M**): Uses Timer 2 of VIA 1 with the Shift Register in one-shot mode to output patterns that generate a single pulse.

  When using this method, PIN_M is implicitly set to output mode, as is PIN_B,
  where Timer 2 clock is emitted.
