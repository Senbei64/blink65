# blink65
Write Arduino-style sketches for VIC-20 and Commodore 64.

## Simple 64
This example uses the same sketch as the blink example,
but has a simplified Makefile for building only the Commodore 64 target,
to show how simple it really is to use the **blink65** library.

Example build command assuming blink65.h and targets are in the current directory:
```
> cl65 -t c64 -o example.prg example.c c64/blink65.lib
```
