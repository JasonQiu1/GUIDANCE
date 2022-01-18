CC := /usr/bin/gcc
SOURCEDIR := src
BUILDDIR := build
INCLUDEDIR := include 
BINARYNAME := gush
BINARYDIR := .

LDFLAGS := -lncurses 
CFLAGS := -Wall -g

SOURCES := $(wildcard $(SOURCEDIR)/*.c)
OBJECTS := $(addprefix $(BUILDDIR)/,$(patsubst %.c, %.o, $(SOURCES:$(SOURCEDIR)/%=%)))

$(BINARYNAME): $(OBJECTS) 
	echo $(SOURCES)
	echo $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINARYNAME) $(CFLAGS) $(LDFLAGS) 

$(BUILDDIR)/event.o: $(SOURCEDIR)/event.c $(SOURCEDIR)/genEventMap.sed $(BUILDDIR)/eventmap.h
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) -I$(BUILDDIR)

$(BUILDDIR)/menu.o: $(SOURCEDIR)/menu.c $(BUILDDIR)/mainmenu.xxd
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) -I$(BUILDDIR)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) 

$(BUILDDIR)/eventmap.h: $(SOURCEDIR)/event.c
	sed -nf $(SOURCEDIR)/genEventMap.sed $< >| $@

XXDPREFIX := $(addsuffix _, $(patsubst /, _, $(SOURCEDIR)))
$(BUILDDIR)/mainmenu.xxd: $(SOURCEDIR)/main.menu
	xxd -i $< | sed -e 's/\([0-9a-f]\)$$/\1, 0x00/' -e "$(addprefix $(addprefix s/, $(XXDPREFIX)), \(.*\)/\1/)" >| $@

clean:
	rm $(BUILDDIR)/*

run:
	./$(BINARYNAME)

.PHONY: run clean
