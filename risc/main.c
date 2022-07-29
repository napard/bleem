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
    risc_map_memory(g_cpu, &device_SCREEN, RISC_CONSOLE_IO_BASE,
        RISC_CONSOLE_IO_LIMIT, RISC_CONSOLE_IO_BASE);
    
    RISC_GETCHAR;
    risc_run(g_cpu);
}
