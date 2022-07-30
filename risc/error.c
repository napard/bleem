/*
 * error.c
 * Error handling stuff.
 *
 * 22 jul 2022 -- 11:53 -03
 * Notes:
 */

#include "risc.h"

void risc_cpu_exception(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...) {
    va_list vargs;
    
    #define CPU pCpu
    printf("\nEXCEPTION at %s(%d): ", pSrcFile, pSrcLine);
    va_start(vargs, pSrcLine);
    vprintf(pMsg, vargs);
    va_end(vargs);
    printf("\n");
    
    printf("IP=%08X  %-10d  SP=%08X FP=%08X FLAGS=%s%s%s%s\n",
        pCpu->registers[reg_IP], pCpu->registers[reg_IP], pCpu->registers[reg_SP], pCpu->registers[reg_FP],
        GET_FLAG(FLAG_Z)? "Z":"-", GET_FLAG(FLAG_V)? "V":"-", GET_FLAG(FLAG_C)? "C":"-", GET_FLAG(FLAG_N)? "N":"-");
    printf("R1=%08X  R2=%08X R3=%08X R4=%08X\n",
        pCpu->registers[reg_R1], pCpu->registers[reg_R2], pCpu->registers[reg_R3], pCpu->registers[reg_R4]);
    printf("R5=%08X  R6=%08X R7=%08X R8=%08X\n",
        pCpu->registers[reg_R5], pCpu->registers[reg_R6], pCpu->registers[reg_R7], pCpu->registers[reg_R8]);
    debug_mem(pCpu->registers[reg_IP], -1);
    exit(retcode_VM_EXCEPTION);
}

void risc_panic(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...) {
    va_list vargs;
    
    printf("PANIC at %s(%d): ", pSrcFile, pSrcLine);
    va_start(vargs, pSrcLine);
    vprintf(pMsg, vargs);
    va_end(vargs);
    printf("\n");
    
    exit(retcode_PANIC);
}
