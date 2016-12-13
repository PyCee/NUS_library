#!/bin/sh

LINK="-lmathe -lm"
FLAGS="-g -pg"

gcc -c main.c

gcc -o mathe_test main.o $LINK $FLAGS

./mathe_test

rm main.o
