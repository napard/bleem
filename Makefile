CC = gcc
PROJECT_FLAGS =
COMMON_CFLAGS = -Wall \
    -Irisc/ \
    $(PROJECT_FLAGS)

LIBS =
PROGS = vmbleem

OBJECTS = \
    risc/devices/screen.o \
	risc/cpu.o \
    risc/error.o \
    risc/exception.o \
    risc/loader.o \
    risc/log.o \
    risc/main.o \
    risc/memmap.o \
    risc/memory.o

ifeq ($(DEBUG),1)
    DBGFLAGS = -g2 -O0 -D_DEBUG
else
    DBGFLAGS = -O2
endif

CFLAGS = $(COMMON_CFLAGS)

CFLAGS += `sdl2-config --cflags`
LDFLAGS += `sdl2-config --libs`

##############################

all: $(PROGS)

$(PROGS): $(OBJECTS)
	$(CC) $(DBGFLAGS) -o $(PROGS) $(OBJECTS) $(LDFLAGS) $(LIBS)

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(CC) $(DBGFLAGS) -o $@ -c $(CFLAGS) $<

clean:
	rm -f $(OBJECTS) $(PROGS)

.PHONY: all
.PHONY: clean
