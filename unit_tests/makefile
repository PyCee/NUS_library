CC=gcc
CFLAGS=-g -Wextra \
	-Wall \
	-Wundef \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wunreachable-code \
	-I/home/kims/VulkanSDK/1.0.39.0/x86_64/include

LFLAGS= -lNUS_library -lNUSM -lxcb -ldl -lm \
	-L/home/kims/VulkanSDK/1.0.39.0/x86_64/lib -lVkLayer_unique_objects \
	-lVkLayer_swapchain -lVkLayer_core_validation -lVkLayer_image \
	-lVkLayer_object_tracker -lVkLayer_parameter_validation -lVkLayer_threading \
	-lassimp

SHADERS:= hello_triangle/triangle_shader/shader.vert.spv hello_triangle/triangle_shader/shader.frag.spv \
	uniform_shader/triangle_shader/shader.vert.spv uniform_shader/triangle_shader/shader.frag.spv \
	textured_cube/triangle_shader/shader.vert.spv textured_cube/triangle_shader/shader.frag.spv

TEST?=3d_math/ 3d_math_vector/ 3d_math_matrix/ 3d_math_axes/ 3d_math_reference_vector/ \
	gravity_simulation/ save_file/ basic_window/ gpu_detection/ vulkan_clear/ \
	hello_triangle/ uniform_shader/ textured_cube/

VALGRIND_RUN= valgrind --tool=memcheck --leak-check=yes --track-origins=yes

EMPTY:= 
SPACE=$(EMPTY) $(EMPTY)

.PHONY: clean

all: recompile

run: compile
	for test in $(TEST:/=/unit_test); do ./$$test; done
shaders: $(SHADERS)
valgrind: $(SHADERS)
	for test in $(TEST:/=/unit_test); do $(VALGRIND_RUN) ./$$test; done
debug: CFLAGS += -D NUS_DEBUG
debug: recompile
compile: $(TEST:/=/unit_test)
	@echo "Files are compiled with flags: $(CFLAGS)"
recompile: clean compile
%/unit_test: %/main.o
	@$(CC) $< $(LFLAGS) -o $@
%.o: %.c
	@$(CC) $(CFLAGS) -c $< $(LFLAGS) -o $@
	@echo "Compiling File: $<"
%.spv:
	@touch $@
	@glslangValidator -V $(@:.spv=$(SPACE)) -o $@
clean:
	@find . -type f \( -name 'unit_test' -o -name '*.o' \) -delete
