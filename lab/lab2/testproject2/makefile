# makefile for OS Exercises: project2 test files
#
#  author:  Dr Ian G Graham - Ian.Graham@griffith.edu.au
#  date:    December 2003
#
# echoesc only needed if printf doesn't work in testp script

CC=gcc
CFLAGS= -Wall 
all: process echoesc 

process: sigtrap.c
	$(CC) $(CFLAGS) $+ -o $@

echoesc: echoesc.c
	$(CC) $(CFLAGS) $+ -o $@
