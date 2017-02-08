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

LFLAGS= -lNUS_library -lxcb -ldl -lm \
	-L/home/kims/VulkanSDK/1.0.33.0/x86_64/lib -lvulkan

UNIT_TESTS:= 3d_math/ xcb_window/ gpu_detection/ vulkan_clear/
UNIT_TESTS:=$(UNIT_TESTS:/=/unit_test)

TEST?=$(UNIT_TESTS)
EMPTY:= 
SPACE=$(EMPTY) $(EMPTY)

.PHONY: clean run valgrind

all: recompile

run: compile
	$(subst $(SPACE), && ,$(addprefix ./, $(TEST:/=/unit_test)))
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./$(TEST)unit_test

compile: $(TEST:/=/unit_test)
recompile: clean compile
%/unit_test: %/main.o
	@echo
	gcc $< $(LFLAGS) -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $<  -o $@
clean:
	find . -type f \( -name 'unit_test' -o -name '*.o' \) -delete
