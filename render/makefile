set-up:
	make compile
	make install
	make clean
compile:
	gcc -c shader_program.c
install:
	sudo ar rcs /usr/local/lib/librender.a shader_program.o
	sudo cp shader_program.h /usr/local/include/
clean:
	rm -f shader_program.o shader_program.o~
