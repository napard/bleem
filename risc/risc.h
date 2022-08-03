/*
 * risc.h
 * Main header.
 *
 * 22 jul 2022 -- 10:24 -03
 * Notes:
 */

#ifndef INCLUDE_RISC_H_
#define INCLUDE_RISC_H_

#ifdef __linux__
#include <signal.h>
#include <unistd.h>
#endif /* __linux__ */

#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compile time flags. -------------------------------------------------------*/

#define RISC_EMULATED_CLOCK
#define RISC_VIDEO_DEVICE_SDL2
#define RISC_SDL2_RENDERER_FLAGS SDL_RENDERER_ACCELERATED

/* Compilation flags dependant includes -------------------------------------*/

#ifdef RISC_VIDEO_DEVICE_SDL2
#include <SDL.h>
#endif /* RISC_VIDEO_DEVICE_SDL2 */

/* ---------------------------------------------------------------------------*/

/* Total RAM bytes. */
#define RISC_TOTAL_MEMORY_BYTES       (1024*1024)
/* Total memory mapped regions. */
#define RISC_TOTAL_MEMORY_REGIONS     8
/* Total device name chars. */
#define RISC_TOTAL_DEVICENAME_CHARS   16

#define RISC_ROM_CODE_BASE            0x00001000
#define RISC_STACK_BASE               0x000ef7f0
#define RISC_CHARSET0_BASE            0x00000800

#define RISC_MALLOC                   malloc
#define RISC_FREE                     free
#define RISC_GETCHAR                  {printf("Press a key...\n"); getchar();}

enum risc_opcode_t {
    opc_NOP =                 0x00,
    opc_MOV_IMM16_REG =       0x10,
    opc_MOV_REG_REG =         0x11,
    opc_MOV_REG_MEM16 =       0x12,
    opc_MOV_MEM16_REG =       0x13,
    opc_MOV_IMM20_REG =       0x14,
    opc_MOV_REG_PTR_REG =     0x15,
    opc_MOV_REG_REG_PTR =     0x16,
#if 0
    opc_MOV_REG_MEM20 =       0x16,
    opc_MOV_MEM20_REG =       0x17,
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
    
    opc_ADDC_REG_REG =        0xc4,
    opc_SUBC_REG_REG =        0xc8,
    
    opc_HALT =                0xff,
    
    RISC_MAX_OPCODES
};

#define ENCODE_INSTR_IMM(i, opc, r1, r2, rest) \
    ((VMWORD *)CPU->ram)[i] = ((opc & 0xff) | ((r1 & 0xf) << 8) | ((r2 & 0xf) << 12) | ((rest & 0xffff) << 16)); \
    printf("INSTR: $%08X\n", ((VMWORD *)CPU->ram)[i]);

#define ENCODE_INSTR_REG(i, opc, r1, r2, r3) \
    ((VMWORD *)CPU->ram)[i] = ((opc & 0xff) | ((r1 & 0xf) << 8) | ((r2 & 0xf) << 12) | ((r3 & 0xf) << 16)); \
    printf("INSTR: $%08X\n", ((VMWORD *)CPU->ram)[i]);

#define STEP_DEBUG debug(CPU->registers[reg_IP]);
#define FETCH32(cpu, v) \
    { \
        v = *((VMWORD*)(cpu->ram + cpu->registers[reg_IP])); \
        CPU->registers[reg_IP] += sizeof(VMWORD); \
    }

#ifndef RISC_EMULATED_CLOCK
#define NEXT_I \
    FETCH32(CPU, fetch); \
    goto *CPU->opctable[fetch & 0xff];
#else
#define NEXT_I \
    goto c_elapsed;
/* TODO: implement instruction cycles and fixed clock rate. */
#define INSTR_CYCLES(opc) 1
#endif /* RISC_EMULATED_CLOCK */

#define RISC_LOG_INFO(cpu, msg, src_file, src_line, ...) risc_log_info(cpu, msg, src_file, src_line, __VA_ARGS__)
//#define RISC_LOG_INFO(cpu, msg, src_file, src_line, ...)

typedef uint32_t VMWORD;    /* VM word. */
typedef int32_t SVMWORD;    /* Signed VM word. */
typedef int16_t SHWORD;     /* Signed half word. */
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

#define GET_FLAG(cpu, f) (cpu ->flags & f)

typedef struct _risc_device_t {
    char name[RISC_TOTAL_DEVICENAME_CHARS];
    uint8_t(*initialize)();
    uint8_t(*finalize)();
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

typedef struct _risc_display_t {
#ifdef RISC_VIDEO_DEVICE_SDL2
    SDL_Window* sdl2_window;
    SDL_Renderer* sdl2_renderer;
    SDL_Texture* sdl2_screen_texture;
#else
#error SDL2 display is not enabled.
#endif /* RISC_VIDEO_DEVICE_SDL2 */
    uint32_t* screen_pixels;
} risc_display_t;

typedef struct _risc_vm_t {
    VMWORD* registers;
    risc_opc_handler_t* opctable;
    VMWORD stack_frame_size;
    VMWORD flags;
#ifdef RISC_EMULATED_CLOCK
    uint32_t cycles;
#endif /* RISC_EMULATED_CLOCK */

    uint8_t* ram;
    risc_memreg_t* memregions;
    uint32_t n_memregions;

    risc_display_t display;
} risc_vm_t;

#define RISC_INSTR_TRACING
#ifdef RISC_INSTR_TRACING
    void debug(risc_vm_t* pCpu, int32_t pAddr);
    #define RISC_TRACE(name) \
        printf("   %s    \n", name); \
        debug(CPU, CPU->registers[reg_IP]); /*\
        RISC_GETCHAR;*/
#else
    #define RISC_TRACE(name)
#endif /* RISC_INSTR_TRACING */
    
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

risc_vm_t* risc_create_cpu();
void risc_destroy_cpu(risc_vm_t* pCpu);
void risc_run();
void debug_mem(risc_vm_t* pCpu, VMWORD pAddr, int32_t pCount);
void debug(risc_vm_t* pCpu, int32_t pAddr);

/* log.c */

void risc_log_info(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...);
void risc_log_warn(risc_vm_t* pCpu, const char* pMsg, const char* pSrcFile, uint32_t pSrcLine, ...);

/* loader.c */

/* Offset of amount of memory per bank, initialized by bootloader. */
#define RISC_LOADER_TOTAL_MEM_LIMIT  0x02
/* Offset of total memory banks installed, initialized by bootloader. */
/*#define RISC_LOADER_TOTAL_MEM_BANKS    0x04*/

void loader_init(risc_vm_t* pCpu, const char* pRomFile);

/* exception.c */

void risc_segfault_sigaction(int32_t pSignal, siginfo_t* pSiginfo,
    void** p_arg);
void risc_fp_exception_sigaction(int32_t pSignal, siginfo_t* pSiginfo,
    void** p_arg);

/* devices/video.c -- Video device */

#include "devices/video.h"

#endif /* INCLUDE_RISC_H_ */
