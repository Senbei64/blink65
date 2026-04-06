100 rem blink65
110 rem copyright 2026 fabio carignano
120 rem --- setup ---
130 dd=56579    :rem cia2 ddrb
140 pr=56577    :rem cia2 prb
150 poke dd,255 :rem pinmode all out
200 rem --- loop ---
210 poke pr,255 :rem digitalwrite high
220 poke 53280,2:rem led on (red)
230 gosub 300   :rem delay
240 poke pr,0   :rem digitalwrite low
250 poke 53280,0:rem led off (black)
260 gosub 300   :rem delay
270 goto 200    :rem loop forever
300 rem --- delay ---
310 for t = 1 to 1000:next t
320 return
