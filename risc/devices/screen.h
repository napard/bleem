/*
 * screen.h
 * Screen output device
 *
 * 28 jul 2022 -- 16:34 -03
 * Notes:
 */

#ifndef INCLUDE_DEVICES_SCREEN_H_
#define INCLUDE_DEVICES_SCREEN_H_

#include "risc.h"

#define RISC_SCREEN_DEVICE_ROWS    25
#define RISC_SCREEN_DEVICE_COLUMNS 40
#define RISC_SCREEN_TILE_PIXELS    8

#define RISC_CONSOLE_IO_BASE        (RISC_TOTAL_MEMORY_BYTES - \
                                    RISC_SCREEN_DEVICE_ROWS * RISC_SCREEN_DEVICE_COLUMNS * 2)
#define RISC_CONSOLE_IO_LIMIT       (RISC_CONSOLE_IO_BASE + \
                                    RISC_SCREEN_DEVICE_ROWS * RISC_SCREEN_DEVICE_COLUMNS * 2 - 1)

extern risc_device_t device_SCREEN;

#endif /* INCLUDE_DEVICES_SCREEN_H_ */

