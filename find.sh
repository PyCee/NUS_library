#!/bin/sh
ALL_FILES=$(find . -name "*.c")
for fie in $ALL_FILES
do
    if grep -q $1 "$fie"; then
	echo $fie
	grep $1 $fie
    fi
done
