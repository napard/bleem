/*
 * cpu.c
 * Central processing unit.
 *
 * 22 jul 2022 -- 10:32 -03
 * Notes:
 */

#include "risc.h"

//#define TEST_SCREEN_DEVICE
//#define INSTR_TEST1
#define INSTR_TEST2

static const char* THIS_FILE = "cpu.h";

risc_vm_t* risc_create_cpu(uint8_t* pRam) {
    risc_vm_t* cpu = RISC_MALLOC(sizeof(risc_vm_t));
    cpu->ram = pRam;
    cpu->registers = (VMWORD*)risc_create_memory(reg_NUM_REGS * sizeof(VMWORD));
    cpu->registers[reg_IP] = RISC_ROM_BASE;
    cpu->registers[reg_SP] = cpu->registers[reg_FP] = RISC_STACK_BASE;
    cpu->stack_frame_size = 0;
    cpu->n_memregions = 0;
    cpu->memregions = RISC_MALLOC(sizeof(risc_memreg_t) * RISC_TOTAL_MEMORY_REGIONS);
    cpu->flags = 0;

    return cpu;
}

void risc_destroy_cpu(risc_vm_t* pCpu) {
    risc_destroy_memory((uint8_t*)pCpu->registers);
    risc_destroy_memory(pCpu->ram);
    RISC_FREE(pCpu->memregions);
    RISC_FREE(pCpu);
}

static risc_vm_t* CPU = NULL;

void debug_mem(VMWORD pAddr, int32_t pCount) {
    int32_t count;
    if(pCount < 0) count = 8; else count = pCount;
    printf("%04X: ", pAddr);
    for(uint32_t i = 0; i < count; i++) {
        printf("%02X ", CPU->ram[i + pAddr]);
    }
    printf("\n\n");
}

void debug(int32_t pAddr) {
    printf("DEBUG -----\n");
    printf("IP=%-8d  SP=%08X FP=%08X FLAGS=%s%s%s%s\n",
        CPU->registers[reg_IP], CPU->registers[reg_SP], CPU->registers[reg_FP],
        GET_FLAG(FLAG_Z)? "Z":"-", GET_FLAG(FLAG_V)? "V":"-", GET_FLAG(FLAG_C)? "C":"-", GET_FLAG(FLAG_N)? "N":"-");
    printf("R1=%08X  R2=%08X R3=%08X R4=%08X\n",
        CPU->registers[reg_R1], CPU->registers[reg_R2], CPU->registers[reg_R3], CPU->registers[reg_R4]);
    printf("R5=%08X  R6=%08X R7=%08X R8=%08X\n",
        CPU->registers[reg_R5], CPU->registers[reg_R6], CPU->registers[reg_R7], CPU->registers[reg_R8]);
    if(pAddr >= 0)
        debug_mem(pAddr, -1);
}

#ifdef TEST_SCREEN_DEVICE
uint32_t i = 0;
void writeToScreen(char pC, char pCommand, VMWORD pPos) {
    CPU->ram[i++] = 0x10 ;
    CPU->ram[i++] = pC ;
    CPU->ram[i++] = pCommand ;
    CPU->ram[i++] = 0x2 ;
    //movlr
    CPU->ram[i++] = 0x12 ;
    CPU->ram[i++] = 0x2 ;
    CPU->ram[i++] = (0x3000 + pPos) & 0xff ;
    CPU->ram[i++] = ((0x3000 + pPos) >> 8) & 0xff ;
    //movrm
} 
#endif /* TEST_SCREEN_DEVICE */

void risc_run(risc_vm_t* pCpu) {

    static risc_opc_handler_t g_opcs0[RISC_MAX_OPCODES];
    for(uint32_t i = 0; i < RISC_MAX_OPCODES; i++)
        g_opcs0[i] = &&__INVALID_OPCODE;

#define OPCODE_VECTOR
#include "opcodes.h"
#undef OPCODE_VECTOR

    CPU = pCpu;
    CPU->opctable = g_opcs0;

#ifdef TEST_SCREEN_DEVICE
    writeToScreen(' ', 0xff, i);
    for (uint32_t i = 0; i < RISC_SCREEN_DEVICE_COLUMNS * RISC_SCREEN_DEVICE_ROWS; i++) {
        writeToScreen('X', 0, i);
    }
    CPU->ram[i++] = opc_HALT;
#endif /* TEST_SCREEN_DEVICE */

#ifdef INSTR_TEST1
    uint32_t i = 0;
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R2, 0, 0x666);i++;
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R7, 0, 0x665);i++;
    ENCODE_INSTR_REG(i, opc_SUB_REG_REG, reg_R2, reg_R7, reg_R3);i++;
    ENCODE_INSTR_REG(i, opc_JMP_GE, 0, 0, 0);i++;
    ENCODE_INSTR_IMM(i, opc_HALT, 0, 0, 0);i++;
#endif /* INSTR_TEST1 */

#ifdef INSTR_TEST2
    uint32_t i = RISC_ROM_BASE / sizeof(VMWORD);
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R2, 0, -9);i++;
    ENCODE_INSTR_IMM(i, opc_MOV_IMM16_REG, reg_R7, 0, 5);i++;
    ENCODE_INSTR_REG(i, opc_MUL_REG_REG, reg_R2, reg_R7, reg_R3);i++;
    ENCODE_INSTR_IMM(i, opc_HALT, 0, 0, 0);i++;
#endif /* INSTR_TEST2 */

    VMWORD fetch;
    FETCH32(CPU, fetch);
    goto *CPU->opctable[fetch & 0xff];

#define OPCODE_IMPL
#include "opcodes.h"
#undef OPCODE_IMPL
}

