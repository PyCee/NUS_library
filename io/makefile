CC=gcc
CFLAGS=-c -Wall -g
SOURCES::=NUS_window.c NUS_system_events.c
OBJECTS::=$(SOURCES:.c=.o)
HEADERS::=$(SOURCES:.c=.h)

all: $(SOURCES) compile
compile: $(OBJECTS)
	sudo ar rcs /usr/local/lib/libio.a $(OBJECTS)
	for header in $(HEADERS); do sudo cp $$header /usr/local/include; done
rebuild: clean all
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@
.PHONY: clean
clean:
	rm *.o -f
	rm *~ -f
