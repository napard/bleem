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

    g_cpu = risc_create_cpu(
        risc_create_memory(RISC_TOTAL_MEMORY_BYTES));

    /* Init 'bootloader' ------------------------------------------------------ */

    loader_init(g_cpu, argv[1]);
    
    /* Init memory mapped I/O ------------------------------------------------- */
    
    /* Initial memory map. */
    risc_map_memory(g_cpu, NULL, 0, RISC_TOTAL_MEMORY_BYTES - 1, 0);
    /* Map and initialize screen device. */
    risc_map_memory(g_cpu, &device_SCREEN, RISC_CONSOLE_IO_BASE,
        RISC_CONSOLE_IO_LIMIT, RISC_CONSOLE_IO_BASE);
    device_SCREEN.initialize();
    
    RISC_GETCHAR;
    risc_run(g_cpu);
}
