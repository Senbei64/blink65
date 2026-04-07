# blink65
Write Arduino-style sketches for VIC-20 and Commodore 64.

### Arduino-style API

This example summarizes the Arduino-style API implemented so far:
```
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
```

### Commodore User Port

|  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| GND |_(1)_|_(1)_|_(1)_|_(1)_|_(1)_|_(1)_|_(1)_|_(1)_|_(1)_|_(1)_| GND |
| GND |_(2)_|I/O 0|I/O 1|I/O 2|I/O 3|I/O 4|I/O 5|I/O 6|I/O 7|_(2)_| GND |
|**A**|**B**|**C**|**D**|**E**|**F**|**H**|**J**|**K**|**L**|**M**|**N**|

_(1)_ Pins of the upper row differ significantly between systems.
Some of them can be used as additional input or output lines.

_(2)_ Pins **B** and **M** of the lower row are available on all systems,
but with different constraints.

I/O pins from **C** to **L** are connected to one port of a VIA or CIA chip on all systems
and can be independently programmed as input or output.
