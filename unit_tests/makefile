CC=gcc
CFLAGS=-g -Wextra \
	-Wall \
	-Wfloat-equal \
	-Wundef \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wconversion \
	-Wunreachable-code \
	-I/home/kims/VulkanSDK/1.0.39.0/x86_64/include

LFLAGS= -lNUS_library -lxcb -ldl -lm \
	-L/home/kims/VulkanSDK/1.0.39.0/x86_64/lib -lVkLayer_unique_objects \
	-lVkLayer_swapchain -lVkLayer_core_validation -lVkLayer_image \
	-lVkLayer_object_tracker -lVkLayer_parameter_validation -lVkLayer_threading

SHADERS:= hello_triangle/triangle_shader/vert.spv hello_triangle/triangle_shader/frag.spv

TEST?=3d_math/ gravity_simulation/ save_file/ basic_window/ gpu_detection/ \
	vulkan_clear/ hello_triangle/

VALGRIND_RUN= valgrind --tool=memcheck --leak-check=yes --track-origins=yes

EMPTY:= 
SPACE=$(EMPTY) $(EMPTY)

.PHONY: clean

all: recompile

run: compile $(SHADERS)
	for test in $(TEST:/=/unit_test); do ./$$test; done
valgrind: $(SHADERS)
	for test in $(TEST:/=/unit_test); do $(VALGRIND_RUN) ./$$test; done
debug: CFLAGS += -D NUS_DEBUG
debug: recompile
compile: $(TEST:/=/unit_test)
recompile: clean compile
%/unit_test: %/main.o
	@$(CC) $< $(LFLAGS) -o $@
%.o: %.c
	@$(CC) $(CFLAGS) -c $<  -o $@
	@echo "Compiling File: $<"
%.spv:
	@touch $@
	@glslangValidator -V $$(dirname $@)/shader.$$(basename --suffix=.spv $@)
	@mv $$(basename $@) $@
clean:
	find . -type f \( -name 'unit_test' -o -name '*.o' -o -name '*.spv' \) -delete
