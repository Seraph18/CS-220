CFLAGS = -g -Wall -std=c18
CC = gcc

#Target for compiling entire program at once

all:  main

main: main.o int-set.o int-set-strings.o
	$(CC) main.o int-set.o int-set-strings.o -o main

main.o: main.c 
	$(CC) $(CFLAGS) main.c 

int-set.o: int-set.c int-set.h
	$(CC) $(CFLAGS) int-set.c

int-set-strings.o: int-set-strings.c int-set-strings.h
	$(CC) $(CFLAGS) int-set-strings.c


#Clean up extra files

clean: cleanFiles clearScreen
	
cleanFiles:
	rm -f *~ *.o tests

clearScreen:
	clear