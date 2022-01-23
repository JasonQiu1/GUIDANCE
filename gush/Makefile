CC := /usr/bin/gcc

GUSHDIR := $(strip $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))))

SOURCEDIR := $(GUSHDIR)/src
BUILDDIR := $(GUSHDIR)/build
INCLUDEDIR := $(GUSHDIR)/include 
BINARYDIR := $(GUSHDIR)
BINARYNAME := gush

LDFLAGS := -lncurses 
CFLAGS := -Wall -g

SOURCES := $(wildcard $(SOURCEDIR)/*.c)
OBJECTS := $(addprefix $(BUILDDIR)/,$(patsubst %.c, %.o, $(SOURCES:$(SOURCEDIR)/%=%)))

$(BINARYDIR)/$(BINARYNAME): $(BUILDDIR) $(OBJECTS) 
	echo $(SOURCES)
	echo $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINARYNAME) $(CFLAGS) $(LDFLAGS) 

$(BUILDDIR):
	@mkdir $(BUILDDIR) || true

$(BUILDDIR)/event.o: $(SOURCEDIR)/event.c $(SOURCEDIR)/genEventMap.sed $(BUILDDIR)/eventmap.h
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) -I$(BUILDDIR)

$(BUILDDIR)/menu.o: $(SOURCEDIR)/menu.c $(BUILDDIR)/mainmenu.xxd
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) -I$(BUILDDIR)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDEDIR) 

$(BUILDDIR)/eventmap.h: $(SOURCEDIR)/event.c
	sed -nf $(SOURCEDIR)/genEventMap.sed $< >| $@

XXDPREFIX := $(subst /,_,$(addsuffix /, $(SOURCEDIR)))
$(BUILDDIR)/mainmenu.xxd: $(SOURCEDIR)/main.menu
	xxd -i $< | sed -e 's/\([0-9a-f]\)$$/\1, 0x00/' -e "$(addprefix $(addprefix s/, $(XXDPREFIX)), \(.*\)/\1/)" >| $@

clean:
	@rm -r $(BUILDDIR) || true > /dev/null 2>&1
	@rm $(BINARYDIR)/$(BINARYNAME) || true >/dev/null 2>&1

run:
	$(BINARYDIR)/$(BINARYNAME)

.PHONY: run clean
