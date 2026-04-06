#- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

SUBDIRS = blink simple64

.PHONY: all clean $(SUBDIRS)

#- BUILD ---------------------------------------------------------------------
all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

#- CLEAN ---------------------------------------------------------------------
clean:
ifeq ($(OS),Windows_NT)
	@for %%d in ($(SUBDIRS)) do $(MAKE) -C %%d clean
else
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
endif

#-----------------------------------------------------------------------------
