/*
 * loader.c
 * Fake bootloader.
 *
 * 25 jul 2022 -- 09:41 -03
 * Notes:
 */

#include "risc.h"

void loader_init(risc_vm_t* pCpu) {
    *((VMWORD*)&pCpu->ram[RISC_LOADER_TOTAL_MEM_LIMIT]) =
        RISC_TOTAL_MEMORY_BYTES - 1;
    *((VMWORD*)&pCpu->ram[RISC_LOADER_TOTAL_MEM_BANKS]) =
        1;
}
