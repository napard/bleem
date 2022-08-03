/*
 * log.c
 * Logging.
 *
 * 24 jul 2022 -- 12:02 -03
 * Notes:
 */

#include "risc.h"

void risc_log_info(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...) {
    va_list vargs;
    
    printf("INFO(%s:%d): ", pSrcFile, pSrcLine);
    va_start(vargs, pSrcLine);
    vprintf(pMsg, vargs);
    va_end(vargs);
    printf("\n");
}

void risc_log_warn(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...) {
    va_list vargs;
    
    printf("WARN(%s:%d): ", pSrcFile, pSrcLine);
    va_start(vargs, pSrcLine);
    vprintf(pMsg, vargs);
    va_end(vargs);
    printf("\n");
}
