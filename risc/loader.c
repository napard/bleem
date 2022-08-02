/*
 * loader.c
 * Fake bootloader.
 *
 * 25 jul 2022 -- 09:41 -03
 * Notes:
 */

#include "risc.h"

static const char* THIS_FILE = "loader.c";

#include "charset0.c"

void loader_init(risc_vm_t* pCpu, const char* pRomFile) {
    *((VMWORD*)&pCpu->ram[RISC_LOADER_TOTAL_MEM_LIMIT]) =
        RISC_TOTAL_MEMORY_BYTES - 1;
    /**((VMWORD*)&pCpu->ram[RISC_LOADER_TOTAL_MEM_BANKS]) =
        1;*/

    /* Load charset 0. */
    memset(&pCpu->ram[RISC_CHARSET0_BASE], 0, 6 * 1024);
    memcpy(&pCpu->ram[RISC_CHARSET0_BASE], charset0, 512);
    
    /* Load rom code from file. */
    FILE* rom;
    if (pRomFile) {
        rom = fopen(pRomFile, "rb");
        if (!rom) {
            risc_panic(pCpu, "unable to open ROM file: %s",
                THIS_FILE, __LINE__, pRomFile);
        }
        fread(&pCpu->ram[RISC_ROM_CODE_BASE], 8*1024, 1, rom);
        fclose(rom);
    }
}
