CC=gcc
CFLAGS=-g -Wall \
	-Wundef \
	-Wshadow \
	-Wfloat-equal \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wunreachable-code \
	-I/home/kims/VulkanSDK/1.0.51.0/x86_64/include

LINK=-lNUS_library -lassimp -lm

OBJ=src/main.o

TARGET=NUSM
EXE:=~/bin/$(TARGET)

rebuild: clean $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $(OBJ) $(LINK)
	@if [ "-a $(EXE)" ] ; then rm $(EXE) ; fi;
	@cp $(TARGET) ~/bin/
.c.o:
	@echo "Compiling File: $<"
	@$(CC) $(CFLAGS) -c $<  -o $@
.PHONY: clean
clean:
	@find . -type f \( -name '*.o' -o -name '*~' \) -delete
