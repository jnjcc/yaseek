## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

PRJROOT = ../..
CSTRUCT = $(PRJROOT)/cstruct
LIBDIR = lib/
INCDIR = include/

OPTS ?= -g -O0
CFLAGS += -Wall -Werror $(OPTS) -I. -I$(CSTRUCT)
LDFLAGS += -L$(CSTRUCT)/darray/lib
LDLIBS += -ldarray

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
LIBDARRAY = libdatrie.a
INCFILE = datrie.h

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
