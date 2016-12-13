
CC=gcc
CFLAGS=-c -Wall -g
LFLAGS=-lphysics -lmathe -lm
TARGET=test

all:
	$(CC) -c main.c $(CFLAGS)
	$(CC) -o library-test main.o $(LFLAGS)
	./library-test
	rm library-test main.o
valgrind:
	$(CC) -c main.c $(CFLAGS)
	$(CC) -o library-test main.o $(LFLAGS)
	valgrind -v --leak-check=full --show-possibly-lost=no ./library-test
	rm library-test main.o
