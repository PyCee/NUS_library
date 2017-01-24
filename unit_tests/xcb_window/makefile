
CC=gcc
CFLAGS=-g -Wall -pedantic \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wconversion

LFLAGS=-lNUS_engine -lxcb -lm

SOURCES=main.c
OBJS=$(SOURCES:.c=.o)
TARGET=unit_test

all: $(SOURCES) compile run
compile: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $(TARGET)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
.PHONY: clean
clean:
	rm *.o -f
	rm *~ -f
rebuild::
	make clean
	make all
run:
	./$(TARGET)

valgrind: $(SOURCES) compile
	valgrind --leak-check=full  ./$(TARGET)
	make clean
