
CC=gcc
CFLAGS=-g -Wall \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wconversion \
	-I/home/kims/VulkanSDK/1.0.33.0/x86_64/include

LFLAGS= -lNUS_library -lX11 -lxcb -lX11-xcb -ldl \
	-L/home/kims/VulkanSDK/1.0.33.0/x86_64/lib -lvulkan

SOURCES=main.c
OBJS=$(SOURCES:.c=.o)

UNIT_TESTS:= xcb_window/ gpu_detection/ #vulkan_clear/
UNIT_TESTS:=$(UNIT_TESTS:/=/unit_test)

RUN_DIR?=gpu_detection/

.PHONY: clean run valgrind

all: recompile

run:
	./$(RUN_DIR)unit_test
valgrind: recompile
	valgrind --leak-check=yes --track-origins=yes ./$(RUN_DIR)unit_test

compile: $(UNIT_TESTS)
recompile: clean compile
%/unit_test: %/main.o
	gcc $< $(LFLAGS) -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $<  -o $@
clean:
	find . -type f \( -name 'unit_test' -o -name '*.o' \) -delete
rebuild:
	make all


.PHONY: clean
