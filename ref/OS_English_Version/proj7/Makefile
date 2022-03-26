CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: main.o 
	$(CC) $(CFLAGS) -o run main.o 

main.o: main.c
	$(CC) $(CFLAGS) -c main.c 

clean:
	rm -rf *.o
	rm -rf example

