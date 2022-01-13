CFLAGS=-Wall -I/mingw64/include/ncurses -lncurses -g

build-gsh:
	gcc main.c -o gsh $(CFLAGS) 

run:
	./gsh

.PHONY: run
