#!/usr/bin/env -S make -f

include ../Makefile.conf

CFLAGS += -pthread -DNTHREAD=$(shell grep processor /proc/cpuinfo | wc -l)
LDFLAGS += -pthread
RUN += trunc-plot
OUT += trunc-plot-func.pdf trunc-plot-raw.pdf trunc-plot.pdf

include ../Makefile.rule

trunc-plot.run: trunc-mt.out.remote
