#!/usr/bin/env -S make -f

include ../Makefile.conf

RUN += enx-plot
OUT += enx-plot.pdf

SHOW += QUADMATH
ifeq ($(shell $(CC) -lquadmath $(LDFLAGS) 2>&1 | grep cannot),)
LDFLAGS += -lquadmath
QUADMATH := true
else
OBJ := $(filter-out enx-quad.o,$(OBJ))
QUADMATH := false
endif

include ../Makefile.rule

enx-plot.run: enx.out
