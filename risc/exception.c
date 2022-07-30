/*
 * exception.c
 * 
 *
 * 30 jul 2022 -- 11:51 -03
 * Notes:
 */

#include "risc.h"

static const char* THIS_FILE = "exception.c";

extern risc_vm_t* g_cpu;

void risc_segfault_sigaction(int32_t pSignal, siginfo_t* pSiginfo,
    void** p_arg)
{
#ifdef __linux__
//    sigrelse(SIGSEGV);
#endif /* __linux__ */

    risc_panic(g_cpu, "SIGSEGV", THIS_FILE, __LINE__);
    //risc_cpu_exception(g_cpu, "EXCEPTION", THIS_FILE, __LINE__);
    abort();
}

void risc_fp_exception_sigaction(int32_t pSignal, siginfo_t* pSiginfo,
    void** p_arg)
{
#ifdef __linux__
//    sigrelse(SIGFPE);
#endif /* __linux__ */

    risc_panic(g_cpu, "SIGFPE", THIS_FILE, __LINE__);
    //risc_cpu_exception(g_cpu, "FLOATING POINT EXCEPTION", THIS_FILE, __LINE__);
    abort();
}
