# blink65
Write Arduino-style sketches with cc65 for VIC-20, PET, and Commodore 64.

### Commodore 64 User Port

|  PIN_1  |  PIN_2  |  PIN_3  |  PIN_4  |  PIN_5  |  PIN_6  |  PIN_7  |  PIN_8  |  PIN_9  |  PIN_10 |  PIN_11 |  PIN_12 |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
|   GND   |  +5V DC |  /RST   |  _(1)_  |  _(1)_  |  _(1)_  |  _(1)_  |  _(1)_  |  _(1)_  | 9V AC + | 9V AC - |   GND   |
|   GND   |  _(1)_  |   PB0   |   PB1   |   PB2   |   PB3   |   PB4   |   PB5   |   PB6   |   PB7   |   PA2   |   GND   |
|**PIN_A**|**PIN_B**|**PIN_C**|**PIN_D**|**PIN_E**|**PIN_F**|**PIN_H**|**PIN_J**|**PIN_K**|**PIN_L**|**PIN_M**|**PIN_N**|

_(1)_ Pins available via direct programming of CIA chips.

### Commodore 64 Variant
The ```tone()``` function uses Timers A and B in free-run mode to toggle pins PB6 (**PIN_K**) and PB7 (**PIN_L**) of CIA 2,
respectively, at the required frequency.

The supported frequency range is between 10 Hz and 65 kHz.

Pins must be explicitly set to output mode.
