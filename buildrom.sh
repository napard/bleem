#!/bin/bash

gforth -e ": bm ; ' bm is bootmessage s\" testrom.fs\" included bye" > testrom.c
gcc -c testrom.c
objcopy --only-section=.data -S -O binary testrom.o testrom.bin



