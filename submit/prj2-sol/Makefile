CFLAGS = -g -Wall -std=c18
CC = gcc
#Target for compiling entire program at once

all:  int-set

int-set: int-set-strings.o int-set.o main.o
	$(CC) $(CFLAGS) int-set-strings.o int-set.o main.o -o int-set

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c 

int-set.o: int-set.c int-set.h
	$(CC) $(CFLAGS) -c int-set.c 

int-set-strings.o: int-set-strings.c int-set-strings.h
	$(CC) $(CFLAGS) -c int-set-strings.c 


#Clean up extra files

clean: cleanFiles clearScreen
	
cleanFiles:
	rm -f *~ *.o tests main

clearScreen:
	clear