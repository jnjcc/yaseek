## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

PRJROOT = ..
LIBDIR = lib/

OPTS ?= -g -O0 
CXXFLAGS += -I. $(OPTS)

SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)
LIBUTLS = libutils.a

all: $(LIBUTLS)
	mkdir -p $(LIBDIR)
	mv $(LIBUTLS) $(LIBDIR)

$(LIBUTLS): $(OBJECTS)
	ar -rs $@ $?

clean:
	rm -rf $(OBJECTS)

distclean:
	rm -rf $(OBJECTS) $(LIBUTLS) $(LIBDIR)
