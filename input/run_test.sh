#!/usr/bin/sh

LINK="-lSDL2 -linput"

gcc -c library_test.c
gcc -o test library_test.o $LINK
rm library_test.o

./test
