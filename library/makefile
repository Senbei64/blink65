#- blink65 - Copyright 2026 Fabio Carignano ----------------------------------

# main make targets
.PHONY: all setup clean

# keep temporary files
.SECONDARY:

# build on Windows and Linux
ifeq ($(OS),Windows_NT)
  # Windows CMD
  MKDIR = mkdir
  RMDIR = rmdir /s /q
  COPY = copy /y
  SEPARATOR = \\
  ERRNUL = 2>nul
else
  # Linux / macOS shell
  MKDIR = mkdir -p
  RMDIR = rm -rf
  COPY = cp -f
  SEPARATOR = /
  ERRNUL = 2>/dev/null
endif

# - BUILD --------------------------------------------------------------------

# target systems
TARGETS = c64 pet vic20

# configurations
CONFIGS = debug release

# all combinations of the library
ALL_LIBS = $(foreach t,$(TARGETS),$(foreach c,$(CONFIGS),\
	build/$(t)/blink65$(if $(filter debug,$(c)),d).lib)\
)

# public headers
HEADERS = blink65.h $(foreach t,$(TARGETS),$(t)/pins.h)

# default target
all: $(ALL_LIBS) $(addprefix build/, $(HEADERS))

# $(1) = one of TARGETS
# $(2) = one of CONFIGS
define BUILD_TEMPLATE
# paths for this combination of target and config
OBJ_DIR_$(1)_$(2) := obj/$(1)/$(2)
LIB_NAME_$(1)_$(2) := build/$(1)/blink65$(if $(filter debug,$(2)),d).lib

# flags for this combination
$$(OBJ_DIR_$(1)_$(2))/%.o: CFLAGS := $(if $(filter debug,$(2)),-DDEBUG=1)

# compile main library sources
$$(OBJ_DIR_$(1)_$(2))/blink65.o: blink65.c blink65.h variant.h | setup
	cl65 -t $(1) $$(CFLAGS) -I. -c -o $$@ $$<

# compile target specific variant
$$(OBJ_DIR_$(1)_$(2))/$(1)variant.s: $(1)/$(1)variant.c | setup
	cc65 -t $(1) $$(CFLAGS) -I. -o $$@ $$<
$$(OBJ_DIR_$(1)_$(2))/$(1)variant.o: $$(OBJ_DIR_$(1)_$(2))/$(1)variant.s | setup
	cl65 -t $(1) $$(CFLAGS) -I. -c -o $$@ $$<

# compose library
$$(LIB_NAME_$(1)_$(2)): $$(OBJ_DIR_$(1)_$(2))/blink65.o\
                        $$(OBJ_DIR_$(1)_$(2))/$(1)variant.o
	ar65 a $$@ $$^
endef

# generate rules for all combinations of targets and configs
$(foreach t,$(TARGETS),\
  $(foreach c,$(CONFIGS),\
    $(eval $(call BUILD_TEMPLATE,$(t),$(c)))\
  )\
)

setup:
	-@$(MKDIR) $(subst /,$(SEPARATOR),\
	  $(foreach t,$(TARGETS),\
	  build/$(t) $(foreach c,$(CONFIGS),\
	  obj/$(t)/$(c)))\
	) $(ERRNUL)

# copy public headers
build/%.h: %.h | setup
	$(COPY) $(subst /,$(SEPARATOR),$<) $(subst /,$(SEPARATOR),$@)

# - CLEAN --------------------------------------------------------------------
clean:
	-$(RMDIR) build obj $(ERRNUL)

#-----------------------------------------------------------------------------
