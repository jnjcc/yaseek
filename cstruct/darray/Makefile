## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

PRJROOT = ../..
LIBDIR = lib/
INCDIR = include/

OPTS ?= -g -O0
CFLAGS += -Wall -Werror -I. $(OPTS)

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
LIBDARRAY = libdarray.a
INCFILE = darray.h

all: $(LIBDARRAY)
	mkdir -p $(LIBDIR) $(INCDIR)
	mv $(LIBDARRAY) $(LIBDIR)
	cp $(INCFILE) $(INCDIR)

$(LIBDARRAY): $(OBJECTS)
	ar -rs $@ $?

clean:
	rm -rf $(OBJECTS)

distclean:
	rm -rf $(OBJECTS) $(LIBDARRAY) $(LIBDIR) $(INCDIR)
