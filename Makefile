CC := gcc
SOURCEDIR := .
BUILDDIR := $(SOURCEDIR)/build
HEADERDIR := $(SOURCEDIR)
INCLUDEDIR := /mingw64/include/ncurses 
BINARY := gsh

LDFLAGS := -lncurses 
CFLAGS := -Wall -g

SOURCES := $(wildcard *.c)
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))

gsh: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINARY) $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) 

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) -I$(HEADERDIR) 

clean:
	rm $(BUILDDIR)/*

run:
	./gsh 2> err

.PHONY: run clean
