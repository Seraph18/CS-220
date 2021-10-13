PROJECT = key-value
SRC_FILES = \
	Makefile \
	README \
	process-file.c \
	process-file.h \
	main.c \
	key-value.c \
	key-value.h
CFLAGS = -g -Wall -std=c18
LDFLAGS = -lm 

$(PROJECT):	key-value.o process-file.o main.o
	$(CC) key-value.o process-file.o main.o $(LDFLAGS) -o $@

clean:
	rm -f *.o $(PROJECT) $(PROJECT).tar.gz

dist:			$(PROJECT).tar.gz

$(PROJECT).tar.gz:	$(SRC_FILES)
			tar -cf $(PROJECT).tar $(SRC_FILES)
			gzip -f $(PROJECT).tar

depend:
	$(CC) -MM $(CFLAGS) *.c

#Auto generate dependencies
key-value.o: key-value.c key-value.h
main.o: main.c process-file.h
process-file.o: process-file.c key-value.h process-file.h

