CC=gcc
CFLAGS=-g -Wall -pedantic \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wconversion

IO_SRC_FILES::=NUS_window.c NUS_system_events.c
IO_DIR::=io/

MATH_SRC_FILES::=vector.c matrix.c quaternion.c pythag.c angle.c cartesian_system.c \
	frustum.c NUS_octree.c
MATH_DIR::=math/

MOD_SRC_FILES::=vertex.c triangle.c texture.c mesh.c model.c joint.c skeleton.c
MOD_DIR::=model/

PHY_SRC_FILES::=center.c physics_state.c movement.c orientation.c kinematic_property.c
PHY_DIR::=physics/

REN_SRC_FILES::=vulkan_wrapper.c
REN_DIR::=render/

IO_SRC::=$(addprefix $(IO_DIR), $(IO_SRC_FILES))
MATH_SRC::=$(addprefix $(MATH_DIR), $(MATH_SRC_FILES))
MOD_SRC::=$(addprefix $(MOD_DIR), $(MOD_SRC_FILES))
PHY_SRC::=$(addprefix $(PHY_DIR), $(PHY_SRC_FILES))
REN_SRC::=$(addprefix $(REN_DIR), $(REN_SRC_FILES))

#NUS_SRC_FILES::=$(IO_SRC) $(MATH_SRC) $(MOD_SRC) $(PHY_SRC) $(REN_SRC)
NUS_SRC_FILES::=$(IO_SRC)
SRC_DIR::=src/
NUS_SRC::=$(addprefix $(SRC_DIR), $(NUS_SRC_FILES))

NUS_OBJ::=$(NUS_SRC:.c=.o)
NUS_HEA_FILES::=io/NUS_window.h io/NUS_system_events.h math/vector.h math/matrix.h \
	math/cartesian_system.h model/model.h physics/center.h
NUS_HEA::=$(addprefix src/, $(NUS_HEA_FILES))

all: $(NUS_SRC) compile

compile: $(NUS_OBJ)
	sudo ar rcs /usr/local/lib/libNUS_engine.a $(NUS_OBJ)
	if [ ! -d "/usr/local/include/NUS" ]; then \
		sudo mkdir /usr/local/include/NUS; fi
	sudo cp $(NUS_HEA) /usr/local/include/NUS
rebuild: clean all

.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@
.PHONY: clean
clean:
	find . -type f \( -name '*.o' -o -name '*~' \) -delete
