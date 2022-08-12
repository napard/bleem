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
//#define RISC_VIRTUAL_GAME_CONSOLE

/* Includes depending on compilation flags. ----------------------------------*/

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

#define RISC_REQUESTED_CYCLES_PER_SEC 8000000

#define RISC_ROM_CODE_BASE            0x00001000
#define RISC_STACK_BASE               0x000ef7f0
#define RISC_CHARSET0_BASE            0x00000800

#define RISC_MALLOC                   malloc
#define RISC_FREE                     free
#define RISC_GETCHAR                  {printf("Press a key...\n"); getchar();}

enum risc_opcode_t {
    opc_NOP =               0x00,

    opc_MOV_I_R =           0x11,
    opc_MOV_R_R =           0x12,
    opc_MOV_R_M =           0x13,
    opc_MOV_M_R =           0x14,
    opc_MOVB_R_M =          0x15,
    opc_MOVB_M_R =          0x16,
    opc_MOVI_R_M =          0x17,
    opc_MOVI_M_R =          0x18,
    opc_MOVIB_R_M =         0x19,
    opc_MOVIB_M_R =         0x1a,
    opc_MOV_MR_R =          0x1b,
    opc_MOVB_MR_R =         0x1c,
    opc_MOV_R_MR =          0x1d,
    opc_MOVB_R_MR =         0x1e,

    opc_ADD =               0x20,
    opc_ADC =               0x21,
    opc_SUB =               0x22,
    opc_SUBC =              0x23,
    opc_INC =               0x24,
    opc_DEC =               0x25,
    opc_MUL =               0x26,
    opc_DIV =               0X27,
    opc_LSHI =              0x28,
    opc_LSHR =              0x29,
    opc_RSHI =              0x2a,
    opc_RSHR =              0x2b,
    opc_AND =               0x2c,
    opc_OR =                0x2d,
    opc_XOR =               0x2e,
    opc_NOT =               0x2f,

    opc_JNE =               0x30, /* |  0000|   00|OPC| */
                                  /* |    16|    8|  0| */
                                  /* |offset|RegMI|OPC| => RegMI = Register = 0x1n / Memory = 0x00 / Indirect = 0x20 */
    opc_JE =                0x31,
    opc_JLT =               0x32,
    opc_JLE =               0x33,
    opc_JGT =               0x34,
    opc_JGE =               0x35,
    opc_JC =                0x36,
    opc_JNC =               0x37,

    opc_PUSH =              0x40,
    opc_POP =               0x41,
    opc_PUSHF =             0x42,
    opc_POPF =              0x43,
    opc_CALL =              0x44,
    opc_RET =               0x45,
    opc_IRET =              0x46,

    opc_HLT =               0xff,

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
/*#define RISC_LOG_INFO(cpu, msg, src_file, src_line, ...)*/

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
    VMWORD registers[reg_NUM_REGS];
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

//#define RISC_INSTR_TRACING
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

/* time.c */

double risc_get_elapsed_time();

/* devices/video.c -- Video device */

#include "devices/video.h"

#endif /* INCLUDE_RISC_H_ */
