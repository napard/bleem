/*
 * screen.c
 * Screen output device.
 *
 * 24 jul 2022 -- 11:44 -03
 * Notes:
 */

#include "risc.h"

static void move_cursor(uint32_t pX, uint32_t pY) {
    printf("\x1b[%d;%dH", pY, pX);
}

static void clear_screen() {
    printf("\x1b[2J");
}

static uint8_t SCREEN_initialize() {
    return 0;
}

uint8_t SCREEN_read_byte(VMWORD pAddr) {
    return 0;
}

void SCREEN_write_byte(uint8_t pData, VMWORD pAddr) {

}

VMWORD SCREEN_read_word(VMWORD pAddr) {
    return 0;
}

void SCREEN_write_word(VMWORD pData, VMWORD pAddr) {
    uint8_t c = pData & 0xff;
    uint8_t cmd = (pData >> 8 & 0xff);
    
    switch(cmd) {
    case 0xff:
        clear_screen();
        break;
    
    default:
        break;
    }
    
    uint8_t x = (pAddr % RISC_SCREEN_DEVICE_COLUMNS) + 1;
    uint8_t y = (pAddr / RISC_SCREEN_DEVICE_COLUMNS) + 1;
    move_cursor(x * 2, y);
    fwrite(&c, 1, 1, stdout);
}

risc_device_t device_SCREEN = {
    "SCREEN",
    SCREEN_initialize,
    SCREEN_read_byte,
    SCREEN_write_byte,
    SCREEN_read_word,
    SCREEN_write_word
};

