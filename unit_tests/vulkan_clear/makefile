
CC=gcc
CFLAGS=-g -Wall -pedantic \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wconversion \
	-I/home/strings/vulkan/VulkanSDK/1.0.33.0/x86_64/include

LFLAGS= -lNUS_engine -lX11 -lxcb -lX11-xcb \
	-L/home/strings/vulkan/VulkanSDK/1.0.33.0/x86_64/lib -lvulkan

SOURCES::=main.c
OBJS::=$(SOURCES:.c=.o)
TARGET::=unit_test

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
	valgrind -v --leak-check=full --show-leak-kinds=all --show-possibly-lost=no --read-var-info=yes --track-origins=yes ./$(TARGET)
	make clean
