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
	-I/home/kims/VulkanSDK/1.0.51.0/x86_64/include

LFLAGS= -lNUS_library -lNUSM -lxcb -ldl -lm \
	-L/home/kims/VulkanSDK/1.0.51.0/x86_64/lib \
	-lVkLayer_unique_objects -lVkLayer_core_validation \
	-lVkLayer_object_tracker -lVkLayer_parameter_validation -lVkLayer_threading \
	-lassimp

SHADERS:= hello_triangle/triangle_shader/shader.vert.spv hello_triangle/triangle_shader/shader.frag.spv \
	uniform_buffer/triangle_shader/shader.vert.spv uniform_buffer/triangle_shader/shader.frag.spv \
	texture/triangle_shader/shader.vert.spv texture/triangle_shader/shader.frag.spv \
	animated_model/triangle_shader/shader.vert.spv animated_model/triangle_shader/shader.frag.spv

TEST?=3d_math/ gravity_simulation/ save_file/ basic_window/ model_load/ gpu_detection/ \
	vulkan_clear/ hello_triangle/ uniform_buffer/ texture/ animated_model/

VALGRIND_RUN= valgrind --tool=memcheck --leak-check=yes --track-origins=yes
EMPTY:= 
SPACE=$(EMPTY) $(EMPTY)

.PHONY: clean

all: recompile

run: $(SHADERS) compile
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
	@echo "Compiled Shader: $@"
clean:
	@find . -type f \( -name 'unit_test' -o -name '*.o' -o -name "*.spv" \) -delete
