CFLAGS=-Wall -lncurses -lreadline -g

build-gsh:
	gcc main.c -o gsh $(CFLAGS) 

run:
	./gsh

.PHONY: run
