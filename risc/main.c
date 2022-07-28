/*
 * main.c
 * 
 *
 * 22 jul 2022 -- 10:17 -03
 * Notes:
 */

#include "risc.h"

risc_vm_t* g_cpu = NULL;

void do_exit() {
    risc_destroy_cpu(g_cpu);
}

int main(int argc, char** argv) {
    atexit(do_exit);
    
    g_cpu = risc_create_cpu(
        risc_create_memory(RISC_TOTAL_MEMORY_BYTES));

    /* Init 'bootloader' ------------------------------------------------------ */

    loader_init(g_cpu);
    
    /* Init memory mapped I/O ------------------------------------------------- */
    
    /* Initial memory map. */
    risc_map_memory(g_cpu, NULL, 0, RISC_TOTAL_MEMORY_BYTES - 1, 0);
    /* Map screen device. */
    /*risc_map_memory(g_cpu, &device_SCREEN, 0x3000,
        0x3000 + RISC_SCREEN_DIVICE_COLUMNS * RISC_SCREEN_DIVICE_ROWS - 1, 0x3000);*/
    
    RISC_GETCHAR;
    risc_run(g_cpu);
}
