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

__OPC_MOV_I_R:  // MOV $12345, R1
{
    r1 = (fetch >> 8) & 0xf;
    SET_REGISTER(r1, (fetch >> 12) & 0xfffff);
    RISC_TRACE("MOV imm20, R1");
    NEXT_I
}

__OPC_MOV_R_R: // MOV R1, R2
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    SET_REGISTER(r2, CPU->registers[r1]);
    RISC_TRACE("MOV R1, R2");
    NEXT_I
}

__OPC_MOV_R_M:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    risc_set_word(CPU, CPU->registers[r2], CPU->registers[r1] + word);
    RISC_TRACE("MOV R2, [R1:imm16]");
    NEXT_I
}

__OPC_MOV_M_R:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    SET_REGISTER(r2, risc_get_word(CPU, CPU->registers[r1] + word));
    RISC_TRACE("MOV [R1:imm16], R2");
    NEXT_I
}

__OPC_MOVB_R_M:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    risc_set_byte(CPU, CPU->registers[r2], CPU->registers[r1] + word);
    RISC_TRACE("MOV BYTE R2, [R1:imm16]");
    NEXT_I
}

__OPC_MOVB_M_R:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    SET_REGISTER(r2, risc_get_byte(CPU, CPU->registers[r1] + word) & 0xff);
    RISC_TRACE("MOV BYTE [R1:imm16], R2");
    NEXT_I
}

__OPC_MOVI_R_M:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    risc_set_word(CPU, CPU->registers[r2], risc_get_word(CPU, CPU->registers[r1] + word));
    RISC_TRACE("MOV R2, <R1:imm16>");
    NEXT_I
}

__OPC_MOVI_M_R:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    SET_REGISTER(r2, risc_get_word(CPU, risc_get_word(CPU, CPU->registers[r1] + word)));
    RISC_TRACE("MOV <R1:imm16>, R2");
    NEXT_I
}

__OPC_MOVIB_R_M:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    risc_set_byte(CPU, CPU->registers[r2], risc_get_word(CPU, CPU->registers[r1] + word));
    RISC_TRACE("MOV BYTE R2, <R1:imm16>");
    NEXT_I
}

__OPC_MOVIB_M_R:
{
    r1 = (fetch >> 8) & 0xf; // base
    r2 = (fetch >> 12) & 0xf; // data
    word = (fetch >> 16) & 0xffff; // offset
    SET_REGISTER(r2, risc_get_word(CPU, risc_get_word(CPU, CPU->registers[r1] + word)) & 0xff);
    RISC_TRACE("MOV BYTE <R1:imm16>, R2");
    NEXT_I
}

__OPC_MOV_MR_R:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    SET_REGISTER(r2, risc_get_word(CPU, CPU->registers[r1]));
    RISC_TRACE("MOV [R1], R2");
    NEXT_I
}

__OPC_MOVB_MR_R:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    SET_REGISTER(r2, risc_get_word(CPU, CPU->registers[r1]) & 0xff);
    RISC_TRACE("MOV BYTE [R1], R2");
    NEXT_I
}

__OPC_MOV_R_MR:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    risc_set_word(CPU, CPU->registers[r1], CPU->registers[r2]);
    RISC_TRACE("MOV R1, [R2]");
    NEXT_I
}

__OPC_MOVB_R_MR:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    risc_set_byte(CPU, CPU->registers[r1], CPU->registers[r2]);
    RISC_TRACE("MOV BYTE R1, [R2]");
    NEXT_I
}

__OPC_ADD:
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
    RISC_TRACE("ADD R1, R2, R3");
    NEXT_I
}

__OPC_ADC:
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
    RISC_TRACE("ADC R1, R2, R3");
    NEXT_I
}

__OPC_SUB:
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
    RISC_TRACE("SUB R2, R1, R3");
    NEXT_I
}

__OPC_SUBC:
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
    RISC_TRACE("SUBC R2, R1, R3");
    NEXT_I
}

__OPC_INC:
{
    r1 = (fetch >> 8) & 0xf;
    SET_REGISTER(r1, CPU->registers[r1] + 1);
    RISC_TRACE("INC R1");
    NEXT_I
}

__OPC_DEC:
{
    r1 = (fetch >> 8) & 0xf;
    SET_REGISTER(r1, CPU->registers[r1] - 1);
    RISC_TRACE("DEC R1");
    NEXT_I
}

__OPC_MUL:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] * CPU->registers[r2]);
    RISC_TRACE("MUL R1, R2, R3");
    NEXT_I
}

__OPC_DIV:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] / CPU->registers[r2]);
    RISC_TRACE("DIV R1, R2, R3");
    NEXT_I
}

__OPC_LSHI:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    word = (fetch >> 16) & 0xffff;
    SET_REGISTER(r2, CPU->registers[r1] << word);
    RISC_TRACE("LSHI imm16, R1, R2");
    NEXT_I
}

__OPC_LSHR:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] << CPU->registers[r2]);
    RISC_TRACE("LSHR R2, R1, R3");
    NEXT_I
}

__OPC_RSHI:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    word = (fetch >> 16) & 0xffff;
    SET_REGISTER(r2, CPU->registers[r1] >> word);
    RISC_TRACE("RSHI imm16, R1, R2");
    NEXT_I
}

__OPC_RSHR:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] >> CPU->registers[r2]);
    RISC_TRACE("RSHR R2, R1, R3");
    NEXT_I
}

__OPC_AND:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] & CPU->registers[r2]);
    RISC_TRACE("AND R1, R2, R3");
    NEXT_I
}

__OPC_OR:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] | CPU->registers[r2]);
    RISC_TRACE("OR R1, R2, R3");
    NEXT_I
}

__OPC_XOR:
{
    r1 = (fetch >> 8) & 0xf;
    r2 = (fetch >> 12) & 0xf;
    r3 = (fetch >> 16) & 0xf;
    SET_REGISTER(r3, CPU->registers[r1] ^ CPU->registers[r2]);
    RISC_TRACE("XOR R1, R2, R3");
    NEXT_I
}

__OPC_NOT:
{
    r1 = (fetch >> 16) & 0xf;
    SET_REGISTER(r1, ~CPU->registers[r1]);
    RISC_TRACE("NOT R1");
    NEXT_I
}

#define JUMP(cond, trace_msg)                                     \
    regmi = (fetch >> 8) & 0xff;                                  \
    if (regmi & 0x10) /* Memory */                                \
    {                                                             \
        soffs = ((fetch >> 16) & 0xffff);                         \
        if (cond)                                                 \
        {                                                         \
            SET_REGISTER(reg_IP, CPU->registers[regmi & 0xf]);    \
        }                                                         \
    }                                                             \
    else if (regmi & 0x20) /* Indirect */                         \
    {                                                             \
        soffs = ((fetch >> 16) & 0xffff);                         \
        if (cond)                                                 \
        {                                                         \
            SET_REGISTER(reg_IP, risc_get_word(CPU, soffs));      \
        }                                                         \
    }                                                             \
    else /* Register */                                           \
    {                                                             \
        soffs = ((fetch >> 16) & 0xffff);                         \
        if (cond)                                                 \
        {                                                         \
            SET_REGISTER(reg_IP, CPU->registers[reg_IP] + soffs); \
        }                                                         \
    }                                                             \
    RISC_TRACE(trace_msg);

__OPC_JNE:
{
    JUMP(!(GET_FLAG(CPU, FLAG_Z)), "JNE")
    NEXT_I
}

__OPC_JE:
{
    JUMP(GET_FLAG(CPU, FLAG_Z), "JE")
    NEXT_I
}

__OPC_JLT:
{
    JUMP(GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V), "JLT")
    NEXT_I
}

__OPC_JLE:
{
    JUMP((GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V)) | GET_FLAG(CPU, FLAG_Z), "JLE")
    NEXT_I
}

__OPC_JGT:
{
    JUMP(!((GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V)) | GET_FLAG(CPU, FLAG_Z)), "JGT")
    NEXT_I
}

__OPC_JGE:
{
    JUMP(!(GET_FLAG(CPU, FLAG_N) ^ GET_FLAG(CPU, FLAG_V)), "JGE")
    NEXT_I
}

__OPC_JC:
{
    JUMP(GET_FLAG(CPU, FLAG_C), "JC")
    NEXT_I
}

__OPC_JNC:
{
    JUMP(!(GET_FLAG(CPU, FLAG_C)), "JNC")
    NEXT_I
}

__OPC_PUSH:
__OPC_POP:

__OPC_PUSHF:
{
    PUSH(CPU->flags)
    NEXT_I
}

__OPC_POPF:
{
    POP(CPU->flags)
    NEXT_I
}

__OPC_CALL:
__OPC_RET:
__OPC_IRET:

__OPC_HLT:
    printf("SYSTEM HALTED\n");
    RISC_GETCHAR;
    exit(0);
    CPU->registers[reg_IP] -= sizeof(VMWORD);
    sleep(5);
    NEXT_I
#endif /* OPCODE_IMPL */

#ifdef OPCODE_VECTOR
#define o(opc) \
    g_opcs0[opc_##opc] = &&__OPC_##opc;
{
    o(NOP)

    o(MOV_I_R)
    o(MOV_R_R)
    o(MOV_R_M)
    o(MOV_M_R)
    o(MOVB_R_M)
    o(MOVB_M_R)
    o(MOVI_R_M)
    o(MOVI_M_R)
    o(MOVIB_R_M)
    o(MOVIB_M_R)
    o(MOV_MR_R)
    o(MOVB_MR_R)
    o(MOV_R_MR)
    o(MOVB_R_MR)

    o(ADD)
    o(ADC)
    o(SUB)
    o(SUBC)
    o(INC)
    o(DEC)
    o(MUL)
    o(DIV)
    o(LSHI)
    o(LSHR)
    o(RSHI)
    o(RSHR)
    o(AND)
    o(OR)
    o(XOR)
    o(NOT)

    o(JNE)  /* |  0000|   00|OPC| */
            /* |    16|    8|  0| */
            /* |offset|RegMI|OPC| => RegMI = Register = 0x1n / Memory = 0x00 / Indirect = 0x20 */
    o(JE)
    o(JLT)
    o(JLE)
    o(JGT)
    o(JGE)
    o(JC)
    o(JNC)

    o(PUSH)
    o(POP)
    o(PUSHF)
    o(POPF)
    o(CALL)
    o(RET)
    o(IRET)

    o(HLT)
}
#endif /* OPCODE_VECTOR */

