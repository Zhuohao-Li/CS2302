
CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: main.o 
	$(CC) $(CFLAGS) -o run main.o  $(PTHREADS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(PTHREADS)

clean:
	rm -rf *.o
	rm -rf example

