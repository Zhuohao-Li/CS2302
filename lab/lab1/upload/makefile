CC=gcc
CFLAGS= -Wall 
PROGNAME=myshell
SOURCE=$(PROGNAME).c 
INCLUDE=

$(PROGNAME): $(SOURCE) $(INCLUDE) makefile
	$(CC) $(CFLAGS) $(SOURCE) -o $@

debug: $(SOURCE) $(INCLUDE) makefile
	$(CC) $(CFLAGS) -g -D DEBUG $(SOURCE) -o $(PROGNAME)
