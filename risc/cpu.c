/*
 * cpu.c
 * Central processing unit.
 *
 * 22 jul 2022 -- 10:32 -03
 * Notes:
 */

#include "risc.h"

//#define INSTR_TEST1
//#define INSTR_TEST2
//#define INSTR_TEST3

static const char* THIS_FILE = "cpu.h";

risc_vm_t* risc_create_cpu() {
    risc_vm_t* cpu = RISC_MALLOC(sizeof(risc_vm_t));
    cpu->ram = risc_create_memory(RISC_TOTAL_MEMORY_BYTES);
    cpu->registers = (VMWORD*)risc_create_memory(reg_NUM_REGS * sizeof(VMWORD));
    cpu->registers[reg_IP] = RISC_ROM_CODE_BASE;
    cpu->registers[reg_SP] = cpu->registers[reg_FP] = RISC_STACK_BASE;
    cpu->stack_frame_size = 0;
    cpu->n_memregions = 0;
    cpu->memregions = RISC_MALLOC(sizeof(risc_memreg_t) * RISC_TOTAL_MEMORY_REGIONS);
    cpu->flags = 0;
#ifdef RISC_EMULATED_CLOCK
    cpu->cycles = 0;
#endif /* RISC_EMULATED_CLOCK */

    return cpu;
}

void risc_destroy_cpu(risc_vm_t* pCpu) {
    risc_destroy_memory((uint8_t*)pCpu->registers);
    risc_destroy_memory(pCpu->ram);
    RISC_FREE(pCpu->memregions);
    RISC_FREE(pCpu);
}

void debug_mem(risc_vm_t* pCpu, VMWORD pAddr, int32_t pCount) {
    int32_t count;
    if(pCount < 0) count = 8; else count = pCount;
    printf("%04X: ", pAddr);
    for(uint32_t i = 0; i < count; i++) {
        printf("%02X ", pCpu->ram[i + pAddr]);
    }
    printf("\n\n");
}

void debug(risc_vm_t* pCpu, int32_t pAddr) {
    printf("TRACE -----\n");
    printf("IP=%08X  %-10d  SP=%08X FP=%08X FLAGS=%s%s%s%s\n",
        pCpu->registers[reg_IP], pCpu->registers[reg_IP], pCpu->registers[reg_SP], pCpu->registers[reg_FP],
        GET_FLAG(pCpu, FLAG_Z)? "Z":"-", GET_FLAG(pCpu, FLAG_V)? "V":"-", GET_FLAG(pCpu, FLAG_C)? "C":"-", GET_FLAG(pCpu, FLAG_N)? "N":"-");
    printf("R1=%08X  R2=%08X R3=%08X R4=%08X\n",
        pCpu->registers[reg_R1], pCpu->registers[reg_R2], pCpu->registers[reg_R3], pCpu->registers[reg_R4]);
    printf("R5=%08X  R6=%08X R7=%08X R8=%08X\n",
        pCpu->registers[reg_R5], pCpu->registers[reg_R6], pCpu->registers[reg_R7], pCpu->registers[reg_R8]);
    if(pAddr >= 0)
        debug_mem(pCpu, pAddr, -1);
}

static risc_vm_t* CPU = NULL;
void risc_run(risc_vm_t* pCpu) {

    static uint8_t r1, r2, r3;
    static SHWORD soffs;
    static VMWORD word, fetch;
    static risc_opc_handler_t g_opcs0[RISC_MAX_OPCODES];
    for(uint32_t i = 0; i < RISC_MAX_OPCODES; i++)
        g_opcs0[i] = &&__INVALID_OPCODE;

#define OPCODE_VECTOR
#include "opcodes.h"
#undef OPCODE_VECTOR

    CPU = pCpu;
    CPU->opctable = g_opcs0;

#ifdef INSTR_TEST1
    uint32_t i = RISC_ROM_BASE / sizeof(VMWORD);
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R2, 0, 0x666);i++;
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R7, 0, 0x665);i++;
    ENCODE_INSTR_REG(i, opc_SUB_REG_REG, reg_R2, reg_R7, reg_R3);i++;
    ENCODE_INSTR_IMM(i, opc_JMP_GE, 0, 0, -12);i++;
    ENCODE_INSTR_IMM(i, opc_HALT, 0, 0, 0);i++;
#endif /* INSTR_TEST1 */

#ifdef INSTR_TEST2
    uint32_t i = RISC_ROM_BASE / sizeof(VMWORD);
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R2, 0, -9);i++;
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R7, 0, 5);i++;
    ENCODE_INSTR_REG(i, opc_MUL_REG_REG, reg_R2, reg_R7, reg_R3);i++;
    ENCODE_INSTR_IMM(i, opc_HALT, 0, 0, 0);i++;
#endif /* INSTR_TEST2 */

#ifdef INSTR_TEST3
    uint32_t i = 4096 ;
    CPU->ram[i++] = 17 ;
    CPU->ram[i++] = 33 ;
    CPU->ram[i++] = 0 ;
    CPU->ram[i++] = 0 ;

    CPU->ram[i++] = 64 ;
    CPU->ram[i++] = 0 ;
    CPU->ram[i++] = 16 ;
    CPU->ram[i++] = 0 ;

    CPU->ram[i++] = 64 ;
    CPU->ram[i++] = 0 ;
    CPU->ram[i++] = 248 ;
    CPU->ram[i++] = 255 ;

    CPU->ram[i++] = 17 ;
    CPU->ram[i++] = 67 ;
    CPU->ram[i++] = 0 ;
    CPU->ram[i++] = 0 ;

    CPU->ram[i++] = 17 ;
    CPU->ram[i++] = 101 ;
    CPU->ram[i++] = 0 ;
    CPU->ram[i++] = 0 ;

    CPU->ram[i++] = 255 ;
    CPU->ram[i++] = 0 ;
    CPU->ram[i++] = 0 ;
    CPU->ram[i++] = 0 ;
#endif /* INSTR_TEST3 */

#ifndef RISC_EMULATED_CLOCK
    FETCH32(CPU, fetch);
    goto *CPU->opctable[fetch & 0xff];
#else
clock:
    if(CPU->cycles == 0) {
        FETCH32(CPU, fetch);
        CPU->cycles = INSTR_CYCLES(fetch & 0xff);
        goto *CPU->opctable[fetch & 0xff];
    }
c_elapsed:
    CPU->cycles--;
    goto clock;    
#endif /* !RISC_EMULATED_CLOCK */

#define OPCODE_IMPL
#include "opcodes.h"
#undef OPCODE_IMPL
}

