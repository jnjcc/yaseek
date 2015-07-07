## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

PRJROOT = ../..
UTILS = $(PRJROOT)/utils
CSTRUCT = $(PRJROOT)/cstruct

OPTS ?= -g -O0
CXXFLAGS += -I$(PRJROOT) $(OPTS)
LDFLAGS += -L$(CSTRUCT)/darray/lib
LDLIBS += -ldarray

SOURCES = $(wildcard *.cc)
APPS = $(SOURCES:.cc=)

all: $(APPS)

clean:
	rm -rf $(APPS)
