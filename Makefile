#!/usr/bin/env -S make -f

DIR = $(shell ls -F | grep /$$)

all run clean:
	@for d in $(DIR); do make $@ -C $$d; done

show:
	@echo DIR = $(DIR)
	@for d in $(DIR); do make $@ -C $$d; done

.PHONY: all run clean show
