CC := /usr/bin/gcc
SOURCEDIR := .
BUILDDIR := $(SOURCEDIR)/build
HEADERDIR := $(SOURCEDIR)
BINARY := gsh

LDFLAGS := -lncurses 
CFLAGS := -Wall -g

SOURCES := $(wildcard *.c)
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))

gsh: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINARY) $(CFLAGS) $(LDFLAGS) 

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(HEADERDIR) 

clean:
	rm $(BUILDDIR)/*

run:
	./gsh 2> err

.PHONY: run clean
