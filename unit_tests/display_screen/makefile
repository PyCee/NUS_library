
CC=gcc
CFLAGS=-g -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith \
	-Wcast-align -Wstrict-prototypes \
	-I/home/strings/vulkan/VulkanSDK/1.0.33.0/x86_64/include
LFLAGS= -L/home/strings/vulkan/VulkanSDK/1.0.33.0/x86_64/lib -lxcb -lvulkan -lio \
	-L/usr/X11R6/lib -lX11

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
