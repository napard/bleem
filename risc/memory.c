/*
 * memory.c
 * Generic memory bank.
 *
 * 22 jul 2022 -- 10:32 -03
 * Notes:
 */

#include "risc.h"

uint8_t* risc_create_memory(uint32_t pSizeInBytes) {
    uint8_t* mem = RISC_MALLOC(pSizeInBytes);
    memset(mem, 0, pSizeInBytes);
    return mem;
}

void risc_destroy_memory(uint8_t* pMem) {
    RISC_FREE(pMem);
}
