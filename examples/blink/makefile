#- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

# path to blink65 library
LIB = ../../library/build

# main make targets
.PHONY: all setup clean

# build on Windows and Linux
ifeq ($(OS),Windows_NT)
  # Windows CMD
  MKDIR = mkdir
  RMDIR = rmdir /s /q
else
  # Linux / macOS shell
  MKDIR = mkdir -p
  RMDIR = rm -rf
endif

# - BUILD --------------------------------------------------------------------

# target systems
TARGETS = c64 pet vic20
SFX_c64   = 64
SFX_pet   = pet
SFX_vic20 = 20
CFG_vic20 = -C vic20-32k.cfg

# configurations
CONFIGS = debug release

ALL_PRGS = $(foreach t,$(TARGETS),\
	$(foreach c,$(CONFIGS),\
		build/blink$(SFX_$(t))$(if $(filter debug,$(c)),d).prg\
	)\
)

# default target
all: $(ALL_PRGS)  

# $(1) = one of TARGETS
# $(2) = one of CONFIGS
define PRG_TEMPLATE
# prg file name
OBJ_$(1)_$(2) := obj/blink$(SFX_$(1))$(if $(filter debug,$(2)),d).o
PRG_$(1)_$(2) := build/blink$(SFX_$(1))$(if $(filter debug,$(2)),d).prg
LIB_$(1)_$(2) := $(LIB)/$(1)/blink65$(if $(filter debug,$(2)),d).lib
FLG_$(1)_$(2) := $(if $(filter debug,$(2)),-DDEBUG=1)

# compile example
$$(OBJ_$(1)_$(2)): blink.c $(LIB)/blink65.h | setup
	cl65 -t $(1) -c $$(FLG_$(1)_$(2)) -I $(LIB) -o $$@ $$<

# link example
$$(PRG_$(1)_$(2)): $$(OBJ_$(1)_$(2)) $$(LIB_$(1)_$(2)) | setup
	cl65 -t $(1) $(CFG_$(1)) -o $$@ $$^
endef

# generate rules for all combinations of targets and configs
$(foreach t,$(TARGETS),\
	$(foreach c,$(CONFIGS),\
		$(eval $(call PRG_TEMPLATE,$(t),$(c)))\
	)\
)

setup:
	-@$(MKDIR) build obj

# - CLEAN --------------------------------------------------------------------
clean:
	-$(RMDIR) build obj

#-----------------------------------------------------------------------------
