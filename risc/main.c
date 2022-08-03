/*
 * main.c
 * 
 *
 * 22 jul 2022 -- 10:17 -03
 * Notes:
 */

#include "risc.h"

risc_vm_t* g_cpu = NULL;

void initialize_devices() {
    device_VIDEORAM.initialize();
    device_VIDEOCTL.initialize();
}

/*void finalize_devices() {
    device_VIDEORAM.finalize();
}*/

void do_exit() {
    risc_destroy_cpu(g_cpu);
}

int main(int argc, char** argv) {
    atexit(do_exit);
    
#ifdef __linux__
    struct sigaction sa;
    // Initialize SIGSEGV exection handler.
    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = (void (*)(int, siginfo_t*, void*)) risc_segfault_sigaction;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);

    // Initialize SIGFPE exection handler.
    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = (void (*)(int, siginfo_t*, void*)) risc_fp_exception_sigaction;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGFPE, &sa, NULL);
#endif /* __linux__ */

    g_cpu = risc_create_cpu();

    /* Init 'bootloader' ------------------------------------------------------ */

    loader_init(g_cpu, argv[1]);
    
    /* Init memory mapped I/O ------------------------------------------------- */
    
    /* Initial memory map. */
    risc_map_memory(g_cpu, NULL, 0, RISC_TOTAL_MEMORY_BYTES - 1, 0);
    /* Map and initialize video device. */
    risc_map_memory(g_cpu, &device_VIDEORAM, RISC_CONSOLE_DATA_BASE,
        RISC_CONSOLE_DATA_LIMIT, RISC_CONSOLE_DATA_BASE);
    risc_map_memory(g_cpu, &device_VIDEOCTL, RISC_VIDEO_CONTROL_REG,
        RISC_VIDEO_CONTROL_REG + 63, RISC_VIDEO_CONTROL_REG);
    
    initialize_devices();
    
    /*RISC_GETCHAR;*/
    debug(g_cpu, g_cpu->registers[reg_IP]);
    risc_run(g_cpu);
}
