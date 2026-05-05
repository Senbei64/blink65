# blink65
Write Arduino-style sketches with cc65 for VIC-20, PET, and Commodore 64.

### Commodore VIC-20 User Port

|  PIN_1  |  PIN_2  |  PIN_3  |  PIN_4  |  PIN_5  |  PIN_6  |  PIN_7  |  PIN_8  |  PIN_9  |  PIN_10 |  PIN_11 |  PIN_12 |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
|   GND   |  +5V DC |  /RST   |   PA2   |   PA3   |   PA4   |   PA5   |   PA6   |   PA7   | 9V AC + | 9V AC - |   GND   |
|   GND   |  _(1)_  |   PB0   |   PB1   |   PB2   |   PB3   |   PB4   |   PB5   |   PB6   |   PB7   |   CB2   |   GND   |
|**PIN_A**|**PIN_B**|**PIN_C**|**PIN_D**|**PIN_E**|**PIN_F**|**PIN_H**|**PIN_J**|**PIN_K**|**PIN_L**|**PIN_M**|**PIN_N**|

_(1)_ Pins available via direct programming of VIA chips.

### Commodore VIC-20 Variant
The ```tone()``` function supports two different output methods depending on the pin:
- PB7 (**PIN_L**): Uses Timer 1 of the 6522 VIA in free-run mode to toggle the pin.
  
  This 16-bit timer allows a very wide frequency range, clamped between 10 Hz and 65 kHz in this implementation.

  The pin must be explicitly set to output mode.
- CB2 (**PIN_M**): uses the VIA Shift Register in free-run mode,
  combining the Timer 2 rate with specific Shift Register patterns to output the required frequency.

  The supported frequency range with this method is between 271 Hz and 65 kHz, on PAL systems, and between 248 Hz and 65 KHz on NTSC systems.

  Using this method, PIN_M is implicitly set to output mode, as is PIN_B, where Timer 2 clock is emitted.
