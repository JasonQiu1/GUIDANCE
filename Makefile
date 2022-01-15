CC := /usr/bin/gcc
SOURCEDIR := .
BUILDDIR := $(SOURCEDIR)/build
HEADERDIR := $(SOURCEDIR)
BINARY := gsh

LDFLAGS := -lncurses 
CFLAGS := -Wall -g

SOURCES := $(wildcard *.c)
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))

gsh: $(BUILDDIR)/mainmenu.xxd $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(BINARY) $(CFLAGS) $(LDFLAGS) 

$(BUILDDIR)/mainmenu.xxd: $(SOURCEDIR)/main.menu
	xxd -i $< | sed 's/\([0-9a-f]\)$$/\1, 0x00/' >| $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(HEADERDIR) -I$(BUILDDIR)

clean:
	rm $(BUILDDIR)/*

run:
	./gsh

.PHONY: run clean
