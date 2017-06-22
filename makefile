CC=gcc
CFLAGS=-g -Wall \
	-Wfloat-equal \
	-Wundef \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wunreachable-code \
	-I/home/kims/VulkanSDK/VulkanSDK/1.0.49.0/x86_64/include

STR_SRC_FILES=NUS_string_group.c NUS_absolute_path.c
STR_DIR=strings

GPU_SRC_FILES=NUS_multi_gpu.c NUS_gpu.c NUS_queue_family.c NUS_submission_queue.c \
	NUS_vulkan_instance.c NUS_binding.c NUS_memory_map.c NUS_single_command.c
GPU_DIR=gpu

IO_SRC_FILES=NUS_window.c NUS_system_events.c
IO_DIR=io

MATH_SRC_FILES=NUS_vector.c NUS_octree.c NUS_matrix.c NUS_axes.c NUS_quaternion.c \
	NUS_frustum.c NUS_reference_vector.c
MATH_DIR=math

MOD_SRC_FILES=NUS_model.c NUS_vertex.c NUS_skeleton.c NUS_joint.c \
	NUS_pose_skeleton.c NUS_pose_joint.c
MOD_DIR=model

PHY_SRC_FILES=NUS_kinematic_property.c NUS_physics_state.c NUS_orientation.c \
	NUS_movement.c NUS_mass.c
PHY_DIR=physics

REN_SRC_FILES=NUS_presentation_surface.c NUS_image_clear.c NUS_shader.c \
	NUS_swapchain.c NUS_texture.c NUS_subpass_info.c NUS_render_pass.c \
	NUS_framebuffer.c NUS_graphics_pipeline.c NUS_depth_buffer.c \
	NUS_pipeline_layout.c
REN_DIR=render

TIME_SRC_FILES=NUS_clock.c
TIME_DIR=time

STR_SRC=$(addprefix $(STR_DIR)/, $(STR_SRC_FILES))
GPU_SRC=$(addprefix $(GPU_DIR)/, $(GPU_SRC_FILES))
IO_SRC=$(addprefix $(IO_DIR)/, $(IO_SRC_FILES))
MATH_SRC=$(addprefix $(MATH_DIR)/, $(MATH_SRC_FILES))
MOD_SRC=$(addprefix $(MOD_DIR)/, $(MOD_SRC_FILES))
PHY_SRC=$(addprefix $(PHY_DIR)/, $(PHY_SRC_FILES))
REN_SRC=$(addprefix $(REN_DIR)/, $(REN_SRC_FILES))
TIME_SRC=$(addprefix $(TIME_DIR)/, $(TIME_SRC_FILES))
OTH_SRC=NUS_vulkan.c NUS_save.c NUS_library.c

STR_HEA=$(STR_SRC:.c=.h)
GPU_HEA=$(GPU_SRC:.c=.h)
IO_HEA=$(IO_SRC:.c=.h)
MATH_HEA=$(MATH_SRC:.c=.h)
MOD_HEA=$(MOD_SRC:.c=.h)
PHY_HEA=$(PHY_SRC:.c=.h)
REN_HEA=$(REN_SRC:.c=.h)
TIME_HEA=$(TIME_SRC:.c=.h)
OTH_HEA=NUS_result.h NUS_os.h NUS_entity.h NUS_key.h \
	NUS_log.h NUS_bool.h $(OTH_SRC:.c=.h)

NUS_SRC_FILES=$(STR_SRC) $(GPU_SRC) $(IO_SRC) $(MATH_SRC) $(MOD_SRC) $(PHY_SRC) \
	$(REN_SRC) $(OTH_SRC) $(TIME_SRC)
NUS_HEA_PRE_PREFIX=$(OTH_HEA) $(NUS_SRC_FILES:.c=.h)
SRC_DIR=src
NUS_SRC=$(addprefix $(SRC_DIR)/, $(NUS_SRC_FILES))
NUS_HEA=$(addprefix $(SRC_DIR)/, $(NUS_HEA_PRE_PREFIX))

NUS_OBJ=$(NUS_SRC:.c=.o)

all: $(NUS_SRC) compile

compile: $(NUS_OBJ)
	@echo "Files are compiled with flags: $(CFLAGS)"

	@if [ -d "/usr/local/include/NUS/" ]; then \
		sudo rm -rf /usr/local/include/NUS/*; fi
	@if [ -f "/usr/local/lib/libNUS_library.a" ]; then \
		sudo rm /usr/local/lib/libNUS_library.a; fi

	@sudo ar rcs /usr/local/lib/libNUS_library.a $(NUS_OBJ)

	@if [ ! -d "/usr/local/include/NUS" ]; then \
		sudo mkdir /usr/local/include/NUS; fi
	@if [ ! -d "/usr/local/include/NUS/$(STR_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(STR_DIR); fi
	@if [ ! -d "/usr/local/include/NUS/$(GPU_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(GPU_DIR); fi
	@if [ ! -d "/usr/local/include/NUS/$(IO_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(IO_DIR); fi
	@if [ ! -d "/usr/local/include/NUS/$(MATH_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(MATH_DIR); fi
	@if [ ! -d "/usr/local/include/NUS/$(MOD_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(MOD_DIR); fi
	@if [ ! -d "/usr/local/include/NUS/$(PHY_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(PHY_DIR); fi
	@if [ ! -d "/usr/local/include/NUS/$(REN_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(REN_DIR); fi
	@if [ ! -d "/usr/local/include/NUS/$(TIME_DIR)" ]; then \
		sudo mkdir /usr/local/include/NUS/$(TIME_DIR); fi
	@sudo cp $(addprefix $(SRC_DIR)/, $(STR_HEA)) /usr/local/include/NUS/$(STR_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(GPU_HEA)) /usr/local/include/NUS/$(GPU_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(IO_HEA)) /usr/local/include/NUS/$(IO_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(MATH_HEA)) /usr/local/include/NUS/$(MATH_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(MOD_HEA)) /usr/local/include/NUS/$(MOD_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(PHY_HEA)) /usr/local/include/NUS/$(PHY_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(REN_HEA)) /usr/local/include/NUS/$(REN_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(TIME_HEA)) /usr/local/include/NUS/$(TIME_DIR)/
	@sudo cp $(addprefix $(SRC_DIR)/, $(OTH_HEA)) /usr/local/include/NUS/
recompile: clean all
.c.o:
	@echo "Compiling File: $<"
	@$(CC) $(CFLAGS) -c $<  -o $@
.PHONY: clean

debug: CFLAGS += -D NUS_DEBUG
debug: recompile
clean:
	@find . -type f \( -name '*.o' -o -name '*~' \) -delete
	@cd unit_tests/; make clean
install:
	sudo apt-get install libx11-xcb-dev libx11-dev libxi-dev \
	libglm-dev graphviz libxcb-dri3-0 libxcb-present0 libpciaccess0 cmake \
	libpng-dev libxcb-dri3-dev libmirclient-dev libwayland-dev \
	xorg-dev build-essential

