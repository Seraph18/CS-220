all: main

main:	main.o file-utils.o morse.o
	gcc main.o file-utils.o morse.o -o morse-encode 
	gcc main.o file-utils.o morse.o -o morse-decode		

main.o: main.c 
		gcc -std=c18 -g -Wall -c main.c

file-utils.o:	file-utils.c file-utils.h
				gcc -std=c18 -g -Wall -c file-utils.c

morse.o:	morse.c morse.h
			gcc -std=c18 -g -Wall -c morse.c

tests: tests.o file-utils.o morse.o
		gcc tests.o file-utils.o morse.o -o tests 

tests.o: tests.c
	gcc -std=c18 -g -Wall -c tests.c
clean: 
		rm -f *~ *.o *.bin morse-decode morse-encode