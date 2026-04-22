#- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

#- BUILD ---------------------------------------------------------------------
blinkbasic64.prg: blinkbasic64.bas
	petcat -w2 -o blinkbasic64.prg -- blinkbasic64.bas

#- CLEAN ---------------------------------------------------------------------
clean:
ifeq ($(OS),Windows_NT)
	-del /q /f blinkbasic64.prg 2>nul
else
	-rm -f blinkbasic64.prg 2>/dev/null
endif

#-----------------------------------------------------------------------------
