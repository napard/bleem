/*
 * risc.h
 * Main header of virtual machine.
 *
 * 22 jul 2022 -- 10:24 -03
 * Notes:
 */

#ifndef INCLUDE_RISC_H_
#define INCLUDE_RISC_H_

#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Total RAM bytes. */
#define RISC_TOTAL_MEMORY_BYTES       (1024*1024)
/* Total memory mapped regions. */
#define RISC_TOTAL_MEMORY_REGIONS     2
/* Total device name chars. */
#define RISC_TOTAL_DEVICENAME_CHARS   16

#define RISC_STACK_BASE               0xff05f

#define RISC_MALLOC                   malloc
#define RISC_FREE                     free
#define RISC_GETCHAR                  {printf("Press a key...\n"); getchar();}

enum risc_opcode_t {
    opc_MOV_IMM16_REG =       0x10,
    opc_MOV_REG_REG =         0x11,
    opc_MOV_REG_MEM =         0x12,
    opc_MOV_MEM_REG =         0x13,
#if 0
    opc_MOV_LIT_MEM =         0x14,
#endif
    opc_MOV_REG_PTR_REG =     0x15,
#if 0
    opc_MOV_LIT_OFF_REG =     0x16,

#endif
    opc_ADD_REG_REG =         0x24,
#if 0
    opc_ADD_LIT_REG =         0x25,
    opc_SUB_LIT_REG =         0x26,
    opc_SUB_REG_LIT =         0x27,
#endif
    opc_SUB_REG_REG =         0x28,
    opc_INC_REG =             0x29,
    opc_DEC_REG =             0x2a,
#if 0
    opc_MUL_LIT_REG =         0x2b,
#endif
    opc_MUL_REG_REG =         0x2c,
    
    opc_LSF_REG_LIT =         0x30,
    opc_LSF_REG_REG =         0x31,
    opc_RSF_REG_LIT =         0x32,
    opc_RSF_REG_REG =         0x33,
#if 0
    opc_AND_REG_LIT =         0x34,
#endif
    opc_AND_REG_REG =         0x35,
#if 0
    opc_OR_REG_LIT =          0x36,
#endif
    opc_OR_REG_REG =          0x37,
#if 0
    opc_XOR_REG_LIT =         0x38,
#endif
    opc_XOR_REG_REG =         0x39,
    opc_NOT =                 0x3a,

    opc_JMP_NOT_EQ =          0x40,
    opc_JMP_EQ =              0x41,
    opc_JMP_LT =              0x42,
    opc_JMP_LE =              0x43,
    opc_JMP_GT =              0x44,
    opc_JMP_GE =              0x45,
    opc_JMP_CY =              0x46,
    opc_JMP_NOT_CY =          0x47,
#if 0
    
    opc_PSH_LIT =             0x57,
    opc_PSH_REG =             0x58,
    
    opc_POP =                 0x59,
    opc_CAL_LIT =             0x6a,
    opc_CAL_REG =             0x6b,
    
    opc_RET =                 0x6c,
#endif
    opc_HALT =                0xff,
    
    RISC_MAX_OPCODES
};

#define FETCH32(cpu, v) \
    { \
        v = *((VMWORD*)(cpu->ram + cpu->registers[reg_IP])); \
    }

#define ENCODE_INSTR_IMM(i, opc, r1, r2, rest) \
    ((VMWORD *)CPU->ram)[i] = ((opc & 0xff) | ((r1 & 0xf) << 8) | ((r2 & 0xf) << 12) | ((rest & 0xffff) << 16)); \
    printf("INSTR: $%08X\n", ((VMWORD *)CPU->ram)[i]);

#define ENCODE_INSTR_REG(i, opc, r1, r2, r3) \
    ((VMWORD *)CPU->ram)[i] = ((opc & 0xff) | ((r1 & 0xf) << 8) | ((r2 & 0xf) << 12) | ((r3 & 0xf) << 16)); \
    printf("INSTR: $%08X\n", ((VMWORD *)CPU->ram)[i]);

#define STEP_DEBUG debug(CPU->registers[reg_IP]);
#define NEXT_I \
    CPU->registers[reg_IP] += sizeof(VMWORD); \
    FETCH32(CPU, fetch); \
    goto *CPU->opctable[fetch & 0xff];

#define RISC_LOG_INFO(cpu, msg, src_file, src_line, ...) risc_log_info(cpu, msg, src_file, src_line, __VA_ARGS__)
//#define RISC_LOG_INFO(cpu, msg, src_file, src_line, ...)

#define RISC_INSTR_TRACING
#ifdef RISC_INSTR_TRACING
    void debug(int32_t pAddr);
    #define RISC_TRACE(name) \
        printf("   %s    \n", name); \
        debug(CPU->registers[reg_IP]); \
        RISC_GETCHAR;
#else
    #define RISC_TRACE(name)
#endif /* RISC_INSTR_TRACING */
    
typedef uint32_t VMWORD;
typedef int32_t  SVMWORD;
typedef void*    risc_opc_handler_t;

enum risc_retcode_t {
    retcode_OK = 0,
    retcode_PANIC = 1,
    retcode_VM_EXCEPTION = 2
};

enum risc_reg_t {
    reg_IP,
    reg_R1,
    reg_R2,
    reg_R3,
    reg_R4,
    reg_R5,
    reg_R6,
    reg_R7,
    reg_R8,
    reg_SP,
    reg_FP,
    reg_NUM_REGS
};

#define FLAG_N 1
#define FLAG_C 2
#define FLAG_V 4
#define FLAG_Z 8

#define GET_FLAG(f) (CPU->flags & f)

typedef struct _risc_device_t {
    char name[RISC_TOTAL_DEVICENAME_CHARS];
    uint8_t(*read_byte)(VMWORD pAddr);
    void(*write_byte)(uint8_t pData, VMWORD pAddr);
    VMWORD(*read_word)(VMWORD pAddr);
    void(*write_word)(VMWORD pData, VMWORD pAddr);
} risc_device_t;

typedef struct _risc_memreg_t {
    risc_device_t* device;
    VMWORD base;
    VMWORD limit;
    VMWORD remap;
} risc_memreg_t;

typedef struct _risc_vm_t {
    uint8_t* ram;
    VMWORD* registers;
    risc_opc_handler_t* opctable;
    risc_memreg_t* memregions;
    uint32_t n_memregions;
    VMWORD stack_frame_size;
    VMWORD flags;
} risc_vm_t;

/* error.c */

void risc_cpu_exception(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...);
void risc_panic(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...);

/* memmap.c */

void risc_map_memory(risc_vm_t* pCpu,
    risc_device_t* pDevice, VMWORD pBase, VMWORD pLimit, VMWORD pRemap);
VMWORD risc_get_word(risc_vm_t* pCpu, VMWORD pAddr);
uint8_t risc_get_byte(risc_vm_t* pCpu, VMWORD pAddr);
void risc_set_word(risc_vm_t* pCpu, VMWORD pVal, VMWORD pAddr);
void risc_set_byte(risc_vm_t* pCpu, uint8_t pVal, VMWORD pAddr);

/* memory.c */

uint8_t* risc_create_memory(uint32_t pSizeInBytes);
void risc_destroy_memory(uint8_t* pMem);

/* cpu.c */

risc_vm_t* risc_create_cpu(uint8_t* pRam);
void risc_destroy_cpu(risc_vm_t* pCpu);
void risc_run();

/* log.c */

void risc_log_info(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...);

/* loader.c */

/* Offset of amount of memory per bank, initialized by bootloader. */
#define RISC_LOADER_TOTAL_MEM_LIMIT    0x02
/* Offset of total memory banks installed, initialized by bootloader. */
#define RISC_LOADER_TOTAL_MEM_BANKS    0x04

void loader_init(risc_vm_t* pCpu);

/* devices/screen.c -- Screen device */

#include "devices/screen.h"

#endif /* INCLUDE_RISC_H_ */
