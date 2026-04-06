#- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

# path to blink65 library
LIB = ../../library/build

#- BUILD ---------------------------------------------------------------------
simple64.prg: simple64.c
	cl65 -t c64 -I $(LIB) -o simple64.prg simple64.c $(LIB)/c64/blink65.lib

#- CLEAN ---------------------------------------------------------------------
clean:
ifeq ($(OS),Windows_NT)
	-del /q /f simple64.prg
else
	-rm -f simple64.o simple64.prg
endif

#-----------------------------------------------------------------------------
