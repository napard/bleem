/*
 * opcodes.h
 * Opcode implementation.
 *
 * 22 jul 2022 -- 11:33 -03
 * Notes:
 */

/*
branchmi -> minus (negative flag set)
branchpl -> negative flag clear
branchcs -> carry flag set
branchcc -> carry flag clear
branchvs -> overflow flag set
branchvc -> overflow flag clear
branchls -> branch less or same (C|Z)
branchhi -> branch hi ~(C|Z)
branchlt -> branch less than (N^V)
branchge -> branch greater or equal ~(N^V)
branchle -> branch less or equal (N^V)|Z
branchgt -> branch greater than ~(N^V)|Z
*/

#define SIGN_BIT_SHIFT ((sizeof(VMWORD) * 8) - 1)

#define PUSH(val) \
    *((VMWORD*)(CPU->ram + CPU->registers[reg_SP])) = val ; \
    CPU->registers[reg_SP] -= sizeof(VMWORD); \
    CPU->stack_frame_size += sizeof(VMWORD);

#define POP(dst) \
    CPU->registers[reg_SP] += sizeof(VMWORD); \
    dst = *((VMWORD*)(CPU->ram + CPU->registers[reg_SP])); \
    CPU->stack_frame_size -= sizeof(VMWORD);

#define DROP() \
    CPU->registers[reg_SP] += sizeof(VMWORD); \
    CPU->stack_frame_size -= sizeof(VMWORD);

#define SAVE_STATE() \
    /* Save CPU state. */ \
    PUSH(CPU->registers[reg_R1]); \
    PUSH(CPU->registers[reg_R2]); \
    PUSH(CPU->registers[reg_R3]); \
    PUSH(CPU->registers[reg_R4]); \
    PUSH(CPU->registers[reg_R5]); \
    PUSH(CPU->registers[reg_R6]); \
    PUSH(CPU->registers[reg_R7]); \
    PUSH(CPU->registers[reg_R8]); \
    /* Save instruction pointer. */ \
    PUSH(CPU->registers[reg_IP]); \
    /* Save stack frame size (including the stack frame size word). */ \
    PUSH(CPU->stack_frame_size + sizeof(VMWORD)); \
    /* Set up frame pointer. */ \
    CPU->registers[reg_FP] = CPU->registers[reg_SP]; \
    CPU->stack_frame_size = 0;

#define RESTORE_STATE() \
    VMWORD frame_pointer_addr = CPU->registers[reg_FP]; \
    CPU->registers[reg_SP] = frame_pointer_addr; \
    VMWORD stack_frame_size; \
    POP(CPU->stack_frame_size); stack_frame_size = CPU->stack_frame_size; \
    /* Restore IP. */ \
    POP(CPU->registers[reg_IP]); \
    /* Restore GPR. */ \
    POP(CPU->registers[reg_R8]); \
    POP(CPU->registers[reg_R7]); \
    POP(CPU->registers[reg_R6]); \
    POP(CPU->registers[reg_R5]); \
    POP(CPU->registers[reg_R4]); \
    POP(CPU->registers[reg_R3]); \
    POP(CPU->registers[reg_R2]); \
    POP(CPU->registers[reg_R1]); \
    /* Delete callee args. */ \
    VMWORD args; \
    POP(args); \
    for(uint32_t i = 0; i < args; i++) {\
        DROP(); \
    } \
    CPU->registers[reg_FP] = frame_pointer_addr + stack_frame_size;

#define SETF_N CPU->flags |= FLAG_N
#define RESETF_N CPU->flags &= ~FLAG_N
#define SETF_C CPU->flags |= FLAG_C
#define RESETF_C CPU->flags &= ~FLAG_C
#define SETF_V CPU->flags |= FLAG_V
#define RESETF_V CPU->flags &= ~FLAG_V
#define SETF_Z CPU->flags |= FLAG_Z
#define RESETF_Z CPU->flags &= ~FLAG_Z

#define SET_REGISTER(r, value) \
    CPU->registers[r] = (value); \
    if(CPU->registers[r]) RESETF_Z; else SETF_Z; \
    if((SVMWORD)CPU->registers[r] >= 0) RESETF_N; else SETF_N;

#ifdef OPCODE_IMPL
__INVALID_OPCODE:
{
    risc_cpu_exception(CPU, "INVALID OPCODE", THIS_FILE, __LINE__);
}

__OPC_NOP:
    NEXT_I

__OPC_MOV_IMM16_REG: // mov imm16, reg
{
    r1 = (fetch >> 8) & 0xf;
    SET_REGISTER(r1, (fetch >> 16) & 0xffff);
    RISC_TRACE("mov imm16, r1");
    NEXT_I
}

__OPC_MOV_IMM20_REG: // mov imm20, reg
{
    r1 = (fetch >> 8) & 0xf;
    SET_REGISTER(r1, (fetch >> 12) & 0xfffff);
    RISC_TRACE("mov imm20, r1");
    NEXT_I
}

__OPC_MOV_REG_REG: // mov reg, reg
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    SET_REGISTER(r2, CPU->registers[r1]);
    RISC_TRACE("mov r1, r2");
    NEXT_I
}

__OPC_MOV_REG_MEM16: // mov r2, [r1:imm16]
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    risc_set_word(CPU, CPU->registers[r2], CPU->registers[r1] + word);
    RISC_TRACE("mov r2, [r1:imm16]");
    NEXT_I
}

__OPC_MOV_MEM16_REG: // mov [r1:imm16], r2
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    SET_REGISTER(r2, risc_get_word(CPU, CPU->registers[r1] + word));
    RISC_TRACE("mov [r1:imm16], r2");
    NEXT_I
}

#if 0
__OPC_MOV_LIT_MEM:
{
    VMWORD val, addr;
    FETCH32(CPU, val);
    FETCH32(CPU, addr);
    risc_set_word(CPU, val, addr);
    NEXT_I
}
#endif

__OPC_MOV_REG_PTR_REG: // mov [r1], r2
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    SET_REGISTER(r2, risc_get_word(CPU, CPU->registers[r1]));
    RISC_TRACE("mov [r1], r2");
    NEXT_I
}

__OPC_MOV_REG_REG_PTR: // mov r1, [r2]
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    risc_set_word(CPU, CPU->registers[r1], CPU->registers[r2]);
    RISC_TRACE("mov r1, [r2]");
    NEXT_I
}

#if 0
__OPC_MOV_LIT_OFF_REG: // mov [addr + r1], r2
{
    VMWORD addr;
    FETCH32(CPU, addr);
    r1 = FETCH(CPU);
    r2 = FETCH(CPU);
    CPU->registers[r2] = risc_get_word(CPU, CPU->registers[r1] + addr);
    NEXT_I
}
#endif

__OPC_ADD_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] + CPU->registers[r2]);
    if(CPU->registers[r3] < CPU->registers[r1])
        SETF_C;
    else
        RESETF_C;

    if(((CPU->registers[r3] ^ CPU->registers[r2]) & (CPU->registers[r3] ^ CPU->registers[r1])) >> SIGN_BIT_SHIFT)
        SETF_V;
    else
        RESETF_V;
    RISC_TRACE("add r1, r2, r3");
    NEXT_I
}

__OPC_ADDC_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] + CPU->registers[r2]);
    if(CPU->registers[r3] < CPU->registers[r1])
        SETF_C;
    else
        RESETF_C;
    CPU->registers[r3] += (GET_FLAG(CPU, FLAG_C) != 0);

    if(((CPU->registers[r3] ^ CPU->registers[r2]) & (CPU->registers[r3] ^ CPU->registers[r1])) >> SIGN_BIT_SHIFT)
        SETF_V;
    else
        RESETF_V;
    RISC_TRACE("addc r1, r2, r3");
    NEXT_I
}

#if 0
__OPC_SUB_LIT_REG:
{
    VMWORD val;
    FETCH32(CPU, val);
    r1 = FETCH(CPU);
    CPU->registers[reg_ACC] = CPU->registers[r1] - val;
    NEXT_I
}

__OPC_SUB_REG_LIT:
{
    VMWORD val;
    r1 = FETCH(CPU);
    FETCH32(CPU, val);
    CPU->registers[reg_ACC] = val - CPU->registers[r1];
    NEXT_I
}
#endif

__OPC_SUB_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] - CPU->registers[r2]);
    if(CPU->registers[r3] > CPU->registers[r1])
        SETF_C;
    else
        RESETF_C;

    if(((CPU->registers[r1] ^ CPU->registers[r2]) & (CPU->registers[r3] ^ CPU->registers[r1])) >> SIGN_BIT_SHIFT)
        SETF_V;
    else
        RESETF_V;
    RISC_TRACE("sub r2, r1, r3");
    NEXT_I
}

__OPC_SUBC_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] - CPU->registers[r2]);
    if(CPU->registers[r3] > CPU->registers[r1])
        SETF_C;
    else
        RESETF_C;
    CPU->registers[r3] -= (GET_FLAG(CPU, FLAG_C) != 0);

    if(((CPU->registers[r1] ^ CPU->registers[r2]) & (CPU->registers[r3] ^ CPU->registers[r1])) >> SIGN_BIT_SHIFT)
        SETF_V;
    else
        RESETF_V;
    RISC_TRACE("subc r2, r1, r3");
    NEXT_I
}

__OPC_INC_REG:
{
    r1 = (fetch >> 8) & 0xf;
    SET_REGISTER(r1, CPU->registers[r1] + 1);
    RISC_TRACE("inc r1");
    NEXT_I
}

__OPC_DEC_REG:
{
    r1 = (fetch >> 8) & 0xf;
    SET_REGISTER(r1, CPU->registers[r1] - 1);
    RISC_TRACE("dec r1");
    NEXT_I
}

#if 0
__OPC_MUL_LIT_REG:
{
    VMWORD val;
    FETCH32(CPU, val);
    r1 = FETCH(CPU);
    CPU->registers[reg_ACC] = CPU->registers[r1] * val;
    NEXT_I
}
#endif

__OPC_MUL_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] * CPU->registers[r2]);
    RISC_TRACE("mul r1, r2, r3");
    NEXT_I
}

__OPC_LSF_REG_LIT:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    word = (fetch >> 16) & 0xffff;
    SET_REGISTER(r2, CPU->registers[r1] << word);
    RISC_TRACE("lsf imm16, r1, r2");
    NEXT_I
}

__OPC_LSF_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] << CPU->registers[r2]);
    RISC_TRACE("lsf r2, r1, r3");
    NEXT_I
}

__OPC_RSF_REG_LIT:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    word = (fetch >> 16) & 0xffff;
    SET_REGISTER(r2, CPU->registers[r1] >> word);
    RISC_TRACE("rsf imm16, r1, r2");
    NEXT_I
}

__OPC_RSF_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] >> CPU->registers[r2]);
    RISC_TRACE("rsf r2, r1, r3");
    NEXT_I
}

#if 0
__OPC_AND_REG_LIT:
{
    VMWORD lit;
    r1 = FETCH(CPU);
    FETCH32(CPU, lit);
    CPU->registers[reg_ACC] = CPU->registers[r1] & lit;
    NEXT_I 
}
#endif

__OPC_AND_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] & CPU->registers[r2]);
    RISC_TRACE("and r1, r2, r3");
    NEXT_I
}

#if 0
__OPC_OR_REG_LIT:
{
    VMWORD lit;
    r1 = FETCH(CPU);
    FETCH32(CPU, lit);
    CPU->registers[reg_ACC] = CPU->registers[r1] | lit;
    NEXT_I 
}
#endif

__OPC_OR_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] | CPU->registers[r2]);
    RISC_TRACE("or r1, r2, r3");
    NEXT_I
}

#if 0
__OPC_XOR_REG_LIT:
{
    VMWORD lit;
    r1 = FETCH(CPU);
    FETCH32(CPU, lit);
    CPU->registers[reg_ACC] = CPU->registers[r1] ^ lit;
    NEXT_I 
}
#endif

__OPC_XOR_REG_REG:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] ^ CPU->registers[r2]);
    RISC_TRACE("xor r1, r2, r3");
    NEXT_I
}

__OPC_NOT:
{
    r1 = (fetch >> 16) & 0xf;
    SET_REGISTER(r1, ~CPU->registers[r1]);
    RISC_TRACE("not r1");
    NEXT_I
}

__OPC_HALT:
{
    printf("SYSTEM HALTED\n");
    RISC_GETCHAR;
    exit(0);
    CPU->registers[reg_IP] -= sizeof(VMWORD);
    sleep(5);
    NEXT_I
}

__OPC_JMP_NOT_EQ:
{
    soffs = ((fetch >> 16) & 0xffff);
    if(!GET_FLAG(CPU, FLAG_Z)) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("jne");
    NEXT_I
}

__OPC_JMP_EQ:
{
    soffs = ((fetch >> 16) & 0xffff);
    if(GET_FLAG(CPU, FLAG_Z)) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("je");
    NEXT_I
}

__OPC_JMP_LT:
{
    soffs = ((fetch >> 16) & 0xffff);
    if(GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V)) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("jlt");
    NEXT_I
}

__OPC_JMP_LE:
{
    soffs = ((fetch >> 16) & 0xffff);
    if((GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V)) | GET_FLAG(CPU, FLAG_Z)) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("jle");
    NEXT_I
}

__OPC_JMP_GT:
{
    soffs = ((fetch >> 16) & 0xffff);
    if(!((GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V)) | GET_FLAG(CPU, FLAG_Z))) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("jgt");
    NEXT_I
}

__OPC_JMP_GE:
{
    soffs = ((fetch >> 16) & 0xffff);
    if(!(GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V))) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("jge");
    NEXT_I
}

__OPC_JMP_CY:
{
    soffs = ((fetch >> 16) & 0xffff);
    if(GET_FLAG(CPU, FLAG_C)) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("jc");
    NEXT_I
}

__OPC_JMP_NOT_CY:
{
    soffs = ((fetch >> 16) & 0xffff);
    if(!GET_FLAG(CPU, FLAG_C)) {
        SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs);
    }
    RISC_TRACE("jnc");
    NEXT_I
}

#if 0
__OPC_PSH_LIT:
{
    VMWORD val;
    FETCH32(CPU, val);
    PUSH(val);
    NEXT_I
}

__OPC_PSH_REG:
{
    r1 = FETCH(CPU);
    PUSH(CPU->registers[r1]);
    NEXT_I
}

__OPC_POP:
{
    r1 = FETCH(CPU);
    POP(CPU->registers[r1]);
    NEXT_I
}

__OPC_CAL_LIT:
{
    VMWORD addr;
    FETCH32(CPU, addr);
    SAVE_STATE();
    /* Set dest IP. */
    CPU->registers[reg_IP] = addr;
    NEXT_I
}

__OPC_CAL_REG:
{
    r1 = FETCH(CPU);
    SAVE_STATE();
    /* Set dest IP. */
    CPU->registers[reg_IP] = r1;
    NEXT_I
}

__OPC_RET:
{
    RESTORE_STATE();
    NEXT_I
}
#endif
#endif /* OPCODE_IMPL */

#ifdef OPCODE_VECTOR
#define o(opc) \
    g_opcs0[opc_##opc] = &&__OPC_##opc;
{
    o(NOP)
    o(MOV_IMM16_REG)
    o(MOV_IMM20_REG)
    o(MOV_REG_REG)
    o(MOV_REG_MEM16)
    o(MOV_MEM16_REG)
#if 0
    o(MOV_LIT_MEM)
#endif
    o(MOV_REG_PTR_REG)
    o(MOV_REG_REG_PTR)
#if 0
    o(MOV_LIT_OFF_REG)
#endif
    o(ADD_REG_REG)
    o(ADDC_REG_REG)
    o(HALT)
#if 0
    o(PSH_LIT)
    o(PSH_REG)
    o(POP)
    o(CAL_LIT)
    o(CAL_REG)
    o(RET)
    o(ADD_LIT_REG)
    o(SUB_LIT_REG)
    o(SUB_REG_LIT)
#endif
    o(SUB_REG_REG)
    o(SUBC_REG_REG)
    o(INC_REG)
    o(DEC_REG)
#if 0
    o(MUL_LIT_REG)
#endif
    o(MUL_REG_REG)
    o(LSF_REG_LIT)
    o(LSF_REG_REG)
    o(RSF_REG_LIT)
    o(RSF_REG_REG)
#if 0
    o(AND_REG_LIT)
#endif
    o(AND_REG_REG)
#if 0
    o(OR_REG_LIT)
#endif
    o(OR_REG_REG)
#if 0
    o(XOR_REG_LIT)
#endif
    o(XOR_REG_REG)
    o(NOT)
    o(JMP_NOT_EQ)
    o(JMP_EQ)
    o(JMP_LT)
    o(JMP_LE)
    o(JMP_GT)
    o(JMP_GE)
    o(JMP_CY)
    o(JMP_NOT_CY)
}
#endif /* OPCODE_VECTOR */

