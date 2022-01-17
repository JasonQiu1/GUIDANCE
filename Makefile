CC := /usr/bin/gcc
SOURCEDIR := .
BUILDDIR := $(SOURCEDIR)/build
HEADERDIR := $(SOURCEDIR)
BINARYNAME := gsh

LDFLAGS := -lncurses 
CFLAGS := -Wall -g

SOURCES := $(wildcard *.c)
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))

gsh: $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(BINARYNAME) $(CFLAGS) $(LDFLAGS) 

$(BUILDDIR)/event.o: $(SOURCEDIR)/event.c $(SOURCEDIR)/genEventMap.sed $(BUILDDIR)/eventmap.h
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(HEADERDIR) -I$(BUILDDIR)

$(BUILDDIR)/menu.o: $(SOURCEDIR)/menu.c $(BUILDDIR)/mainmenu.xxd
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(HEADERDIR) -I$(BUILDDIR)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(HEADERDIR) 

$(BUILDDIR)/eventmap.h: $(SOURCEDIR)/event.c
	sed -nf $(SOURCEDIR)/genEventMap.sed $< >| $@

$(BUILDDIR)/mainmenu.xxd: $(SOURCEDIR)/main.menu
	xxd -i $< | sed 's/\([0-9a-f]\)$$/\1, 0x00/' >| $@

clean:
	rm $(BUILDDIR)/*

run:
	./gsh

.PHONY: run clean
