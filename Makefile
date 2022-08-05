CC = gcc
PROJECT_FLAGS =
COMMON_CFLAGS = -Wall \
    -Irisc/ \
    $(PROJECT_FLAGS)

LIBS =
PROGS = vmbleem

OBJECTS = \
    risc/devices/video.o \
    risc/cpu.o \
    risc/error.o \
    risc/exception.o \
    risc/loader.o \
    risc/log.o \
    risc/main.o \
    risc/memmap.o \
    risc/memory.o \
    risc/time.o

ifeq ($(DEBUG),1)
    DBGFLAGS = -g2 -O0 -D_DEBUG
else
    DBGFLAGS = -O2
endif

CFLAGS = $(COMMON_CFLAGS)

CFLAGS += `sdl2-config --cflags`
LDFLAGS += `sdl2-config --libs` -lm

##############################

all: $(PROGS)

$(PROGS): $(OBJECTS)
	$(shell ./buildcharset.sh)
	$(shell ./buildrom.sh)
	$(CC) $(DBGFLAGS) -o $(PROGS) $(OBJECTS) $(LDFLAGS) $(LIBS)

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(CC) $(DBGFLAGS) -o $@ -c $(CFLAGS) $<

clean:
	rm -f $(OBJECTS) $(PROGS)
	rm -f testrom.c testrom.o testrom.bin

.PHONY: all
.PHONY: clean
