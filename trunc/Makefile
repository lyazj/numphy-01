#!/usr/bin/env -S make -f

include ../Makefile.conf

CFLAGS += -pthread -DNTHREAD=$(shell grep processor /proc/cpuinfo | wc -l)
LDFLAGS += -pthread

include ../Makefile.rule
