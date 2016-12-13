#!bin/sh

LINK="-lmodels -lSDL2 -lGL -lGLEW -lassimp -lmathe -lm -lSOIL"
FLAG="-Wall"

gcc -c main.c

gcc -o unit_test main.o $FLAG $LINK

rm main.o

./unit_test
echo Done
