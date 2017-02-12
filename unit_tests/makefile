

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

SHADERS_VERT != find . -type f -name "*.vert"
SHADERS_FRAG != find . -type f -name "*.frag"
SHADERS:= hello_triangle/triangle_shader/vert.spv hello_triangle/triangle_shader/frag.spv

TEST?=3d_math/ xcb_window/ gpu_detection/ vulkan_clear/ hello_triangle/

EMPTY:= 
SPACE=$(EMPTY) $(EMPTY)

.PHONY: clean

all: recompile

run: compile $(SHADERS)
	for test in $(TEST:/=/unit_test); do ./$$test; done
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./$(TEST)unit_test

compile: $(TEST:/=/unit_test)
recompile: clean compile
%/unit_test: %/main.o
	@echo
	gcc $< $(LFLAGS) -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $<  -o $@
%.spv:
	@touch $@
	@echo $$(dirname $@)/shader.$$(basename --suffix=.spv $@)
	glslangValidator -V $$(dirname $@)/shader.$$(basename --suffix=.spv $@) # > $@
clean:
	find . -type f \( -name 'unit_test' -o -name '*.o' -o -name '*.spv' \) -delete
