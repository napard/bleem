/*
 * cpu.c
 * Central processing unit.
 *
 * 22 jul 2022 -- 10:32 -03
 * Notes:
 */

#include "risc.h"

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

    static double fixed_time_step = 1000.0 / RISC_REQUESTED_CYCLES_PER_SEC;
    static double timepoint1, timepoint2, lag = 0.0;
    static float last_elapsed;
    static SHWORD soffs;
    static VMWORD word, fetch;
    static risc_opc_handler_t g_opcs0[RISC_MAX_OPCODES];
    static uint8_t r1, r2, r3;
#ifdef _DEBUG        
    static uint32_t instr_per_secs = 0;
    static double accum_time = 0.0;
#endif /* _DEBUG */
    
    /* Clear opcode table. */
    for(uint32_t i = 0; i < RISC_MAX_OPCODES; i++)
        g_opcs0[i] = &&__INVALID_OPCODE;

#define OPCODE_VECTOR
#include "opcodes.h"
#undef OPCODE_VECTOR

    CPU = pCpu;
    CPU->opctable = g_opcs0;

    timepoint1 = timepoint2 = risc_get_elapsed_time();

#ifndef RISC_EMULATED_CLOCK
    FETCH32(CPU, fetch);
    goto *CPU->opctable[fetch & 0xff];
#else
clock:
    timepoint1 = risc_get_elapsed_time();
    float elapsed = (timepoint1 - timepoint2);
    last_elapsed = elapsed * 1000;
    lag += last_elapsed;
    
    if(lag > fixed_time_step) {
        if(CPU->cycles == 0) {
#ifdef _DEBUG
        instr_per_secs++;
#endif /* _DEBUG */
            FETCH32(CPU, fetch);
            CPU->cycles = INSTR_CYCLES(fetch & 0xff);
            goto *CPU->opctable[fetch & 0xff];
        }
c_elapsed:
        lag -= fixed_time_step;
        CPU->cycles--;
    }
    timepoint2 = timepoint1;
#ifdef _DEBUG        
        accum_time += last_elapsed;
        if(accum_time > 1000.0) {
            accum_time = 0.0;
            printf("INSTR PER SECS: %d\n", instr_per_secs);
            instr_per_secs = 0;
        }
#endif /* _DEBUG */
    goto clock;
#endif /* !RISC_EMULATED_CLOCK */

#define OPCODE_IMPL
#include "opcodes.h"
#undef OPCODE_IMPL
}

