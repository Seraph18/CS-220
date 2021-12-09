COURSE = cs220

TARGET = cache-sim

CPPFLAGS = -I $(HOME)/$(COURSE)/include
CFLAGS = -g -Wall -std=c18

LIBDIR = $$HOME/$(COURSE)/lib
LIB = cs220

LDFLAGS = -L $(LIBDIR)
LDLIBS = -l$(LIB)

OBJS = \
  cache-sim.o \
  main.o 

$(TARGET):	$(OBJS)
		$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -Wl,-rpath=$(LIBDIR) -o $@

lu: cache-sim extras/lru_2-2-2-8.test
	sed -e 's/#.*//' extras/lru_2-2-2-8.test \
        | ./cache-sim -r lru -v 2-2-2-8


ru: cache-sim extras/mru_4-2-8-16.test
	sed -e 's/#.*//' extras/mru_4-2-8-16.test \
        | ./cache-sim -r mru -v 4-2-8-16

#Cleanup

clean:		
		rm -f $(OBJS) $(TARGET) *~

wipeScreen:
	clear

clear: clean wipeScreen


