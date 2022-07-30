/*
 * loader.c
 * Fake bootloader.
 *
 * 25 jul 2022 -- 09:41 -03
 * Notes:
 */

#include "risc.h"

static const char* THIS_FILE = "loader.c";

void loader_init(risc_vm_t* pCpu, const char* pRomFile) {
    *((VMWORD*)&pCpu->ram[RISC_LOADER_TOTAL_MEM_LIMIT]) =
        RISC_TOTAL_MEMORY_BYTES - 1;
    /**((VMWORD*)&pCpu->ram[RISC_LOADER_TOTAL_MEM_BANKS]) =
        1;*/

    FILE* rom;
    if (pRomFile) {
        rom = fopen(pRomFile, "rb");
        if (!rom) {
            risc_panic(pCpu, "unable to open ROM file: %s",
                THIS_FILE, __LINE__, pRomFile);
        }
        fread(&pCpu->ram[RISC_ROM_BASE], 8*1024, 1, rom);
        fclose(rom);
    }
}
