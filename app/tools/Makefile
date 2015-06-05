## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

PRJROOT = ../..
UTILS = $(PRJROOT)/utils

OPTS ?= -g -O0
CXXFLAGS += -I$(PRJROOT) $(OPTS)

SOURCES = $(wildcard *.cc)
APPS = $(SOURCES:.cc=)

all: $(APPS)

clean:
	rm -rf $(APPS)
