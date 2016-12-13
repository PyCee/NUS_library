make:
	make compile
	make install
	make clean
compile:
	gcc -c vertex.c
	gcc -c triangle.c
	gcc -c texture.c
	gcc -c mesh.c
	gcc -c model.c
	gcc -c joint.c
	gcc -c skeleton.c

install:
	sudo ar rcs /usr/local/lib/libmodels.a vertex.o triangle.o texture.o mesh.o joint.o triangle.o model.o
	sudo cp vertex.h /usr/local/include/
	sudo cp triangle.h /usr/local/include/
	sudo cp texture.h /usr/local/include/
	sudo cp mesh.h /usr/local/include/
	sudo cp model.h /usr/local/include/
	sudo cp joint.h /usr/local/include/
	sudo cp skeleton.h /usr/local/include/

clean:
	rm vertex.o
	rm triangle.o
	rm texture.o
	rm mesh.o
	rm model.o
	rm joint.o
	rm skeleton.o
