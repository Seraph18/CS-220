PROJECT = quadr
SRC_FILES = \
	Makefile \
	README \
	test.data \
	main.c \
	quadr.c \
	quadr.h
CFLAGS = -g -Wall -std=c18
LDFLAGS = -lm 

$(PROJECT):	quadr.o main.o
	$(CC) quadr.o main.o $(LDFLAGS) -o $@

clean:
	rm -f *.o $(PROJECT) $(PROJECT).tar.gz

dist:			$(PROJECT).tar.gz

$(PROJECT).tar.gz:	$(SRC_FILES)
			tar -cf $(PROJECT).tar $(SRC_FILES)
			gzip -f $(PROJECT).tar

depend:
	$(CC) -MM $(CFLAGS) *.c

#Auto generate dependencies
main.o: main.c quadr.h
quadr.o: quadr.c quadr.h
