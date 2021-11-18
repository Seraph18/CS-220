COURSE = cs220

COURSE_DIR = $(HOME)/$(COURSE)

TARGET = fn-trace 

CFLAGS = -g -Wall -I $(COURSE_DIR)/include -Og -std=c18 -fPIC

LDLIBS =  -l cs220 -lcapstone -ldl
LDFLAGS = -L $(COURSE_DIR)/lib

OBJS = \
  fn-trace.o \
  main.o \
  x86-64_lde.o\
  fns

$(TARGET):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

fns:		fns.c
		$(CC) $(CFLAGS) -shared $< -o $@

r: fn-trace
	./fn-trace fns testFn

clean:		
		rm -f $(OBJS) $(TARGET) fns fns.o *~

wipeTerminal:
	clear

clear: clean wipeTerminal

DEPENDS:
		gcc $(CFLAGS) -MM *.c

#make DEPENDS output with cs220/include dependencies removed
fn-trace.o: fn-trace.c fn-trace.h x86-64_lde.h 
main.o: main.c fn-trace.h
x86-64_lde.o: x86-64_lde.c x86-64_lde.h
