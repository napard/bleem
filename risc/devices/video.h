/*
 * video.h
 * Video output device
 *
 * 28 jul 2022 -- 16:34 -03
 * Notes:
 */

#ifndef INCLUDE_DEVICES_VIDEO_H_
#define INCLUDE_DEVICES_VIDEO_H_

#include "risc.h"

#define RISC_VIDEO_SCALER         3
#define RISC_VIDEO_DEVICE_ROWS    25
#define RISC_VIDEO_DEVICE_COLUMNS 40
#define RISC_VIDEO_TILE_PIXELS    8

#define RISC_CONSOLE_DATA_BASE      (RISC_TOTAL_MEMORY_BYTES - \
                                    RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_DEVICE_COLUMNS * 2)
#define RISC_CONSOLE_DATA_LIMIT     (RISC_CONSOLE_DATA_BASE + \
                                    RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_DEVICE_COLUMNS * 2 - 1)

#define RISC_VIDEO_CONTROL_BASE     0x000ff7f0
#define RISC_VIDEO_CMD_REG          0x00000000
#define RISC_VIDEO_BGCOL_REG        0x00000001
#define RISC_VIDEO_FGCOL_REG        0x00000002

extern risc_device_t device_VIDEORAM;
extern risc_device_t device_VIDEOCTL;

typedef struct _point2_t
{
    int16_t x;
    int16_t y;
} point2_t;

point2_t point2_normalize(point2_t pP);

#endif /* INCLUDE_DEVICES_VIDEO_H_ */

