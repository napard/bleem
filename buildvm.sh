#!/bin/bash

reset
gcc -g -O0 -Wall -o vmbleem -Irisc/ \
    risc/main.c risc/cpu.c risc/error.c \
    risc/memmap.c risc/memory.c risc/log.c \
    risc/loader.c risc/exception.c \
    risc/devices/screen.c `sdl2-config --cflags --libs`
