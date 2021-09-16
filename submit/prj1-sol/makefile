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


clean: 
		rm -f *~ *.o morse-decode morse-encode