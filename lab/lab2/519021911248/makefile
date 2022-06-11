# makefile for OS Exercises: exercise 7
#
#  author:  Haoyuan Fu
#  date:    March, 2022
#

CC=gcc
CFLAGS= -Wall 
PROGNAME=hostd
SOURCE=src/$(PROGNAME).c src/pcb.c src/mab.c src/rsrc.c
INCLUDE=src/$(PROGNAME).h src/pcb.h src/mab.h src/rsrc.h

$(PROGNAME): $(SOURCE) $(INCLUDE) makefile process
	$(CC) $(CFLAGS) $(SOURCE) -o $@

debug: $(SOURCE) $(INCLUDE) makefile process
	$(CC) $(CFLAGS) -g -D DEBUG $(SOURCE) -o $(PROGNAME)

process: src/sigtrap.c
	$(CC) $(CFLAGS) $+ -o $@

clean:
	rm ./hostd ./process
