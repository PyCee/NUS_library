CC=gcc
CFLAGS=-c -Wall -g
SOURCES::=center.c physics_state.c movement.c orientation.c kinematic_property.c
OBJECTS::=$(SOURCES:.c=.o)
HEADERS::=$(SOURCES:.c=.h)

all: $(SOURCES) compile
compile: $(OBJECTS)
	sudo ar rcs /usr/local/lib/libphysics.a $(OBJECTS)
	for header in $(HEADERS); do sudo cp $$header /usr/local/include; done
rebuild: clean all
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@
.PHONY: clean
clean:
	rm *.o -f
	rm *~ -f
