#!/usr/bin/env -S make -f

include ../Makefile.conf

# CFLAGS := $(subst -O3,-Og,$(CFLAGS))
BIN := $(filter-out gem cholesky,$(BIN))
PLT := $(shell ls *-plot)
RUN += $(PLT)
OUT += $(PLT:%=%.pdf) hx1-plot-13.pdf

include ../Makefile.rule

det-plot.run: det.out
hx1-plot.run: hx1.out
gem-test hx1: gem.o
cholesky-test cdm-test hx1: cholesky.o
cdm-test.o cholesky.o cholesky-test.o gem.o gem-test.o hx1.o: defs.h
