# blink65
Blink65 is a project created and maintained by Fabio Carignano.

Write Arduino-style sketches with cc65 for VIC-20, PET, and Commodore 64.

## IR Loop
Using the tone() and digitalRead() functions with an infrared (IR) LED and receiver.

This sketch drives an IR LED with 10 ms bursts of a 38 kHz carrier for 5 seconds, then keeps it silent for 5 seconds, cyclically.
The output of an IR receiver (VS1838) is shown on the built-in LED, to verify that transmission and reception work.
