CC=gcc
CFLAGS=-c -Wall -g
SOURCES::=vector.c matrix.c quaternion.c pythag.c angle.c cartesian_system.c frustum.c
OBJECTS::=$(SOURCES:.c=.o)
HEADERS::=$(SOURCES:.c=.h)

all: $(SOURCES) compile
compile: $(OBJECTS)
	sudo ar rcs /usr/local/lib/libmathe.a $(OBJECTS)
	for header in $(HEADERS); do sudo cp $$header /usr/local/include; done
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@
.PHONY: clean
clean:
	rm *.o -f
	rm *~ -f
