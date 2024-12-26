/**                                                                     -*- C -*-
 * @file        code.inc
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 *
 * @brief       This file is an x-macro header file, designed to be
 *              included more than once. Its use can change depending
 *              how is defined the corresponed x-macro.
 */

#ifndef cloxDefineOpCode
#   define cloxDefineOpCode(...)
#endif

/**
 * @defgroup    OP_CODES OpCodes
 * @{
 */

/* =---- Status Control OpCodes --------------------------------= */

/**
 * @brief       Represents 'break' opcode (breakpoint).
 * 
 * @note        This opcode sends to the attached host a breakpoint signal,
 *              this opcode is useful only when the attached host has a debug
 *              purpose.
 */
cloxDefineOpCode(CLOX_OP_CODE_BREAK,    0x01,   "break",    CLOX_OP_KIND_BYTE,  _op_break)
/**
 * @brief       Represents 'abort' opcode (abort).
 * 
 * @note        This opcode sends to the attached host an abort signal that
 *              causes a forced execution termination.
 */
cloxDefineOpCode(CLOX_OP_CODE_ABORT,    0x02,   "abort",    CLOX_OP_KIND_BYTE,  _op_abort)
/**
 * @brief       Represents 'exit' opcode (exit).
 * 
 * @note        This opcode sets the termination code and sends to the attached
 *              host a signal that makes it terminate program execution.
 */
cloxDefineOpCode(CLOX_OP_CODE_EXIT,     0x03,   "exit",     CLOX_OP_KIND_CTRL,  _op_exit)
/**
 * @brief       Represents 'raise' opcode (raise).
 * 
 * @note        This opcode sends to the attached host a signal directly specified
 *              in the instruction.
 */
cloxDefineOpCode(CLOX_OP_CODE_RAISE,    0x04,   "raise",    CLOX_OP_KIND_CTRL,  _op_raise)

/* =---- Branching OpCodes -------------------------------------= */

/**
 * Jump OpCodes
 */

/**
 * @brief       Represents 'jmp' opcode (jump).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter.
 */
cloxDefineOpCode(CLOX_OP_CODE_JMP,      0x10,   "jmp",      CLOX_OP_KIND_JUMP,  _op_jmp)
/**
 * @brief       Represents 'jit' opcode (jump if true).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the zero flag (ZF) is zero.
 */
cloxDefineOpCode(CLOX_OP_CODE_JIT,      0x11,   "jit",      CLOX_OP_KIND_JUMP,  _op_jit)
/**
 * @brief       Represents 'jnt' opcode (jump if not true).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the zero flag (ZF) is one.
 */
cloxDefineOpCode(CLOX_OP_CODE_JNT,      0x12,   "jnt",      CLOX_OP_KIND_JUMP,  _op_jnt)
/**
 * @brief       Represents 'jeq' opcode (jump if equal).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the comparison flag (CF) is zero.
 */
cloxDefineOpCode(CLOX_OP_CODE_JEQ,      0x13,   "jeq",      CLOX_OP_KIND_JUMP,  _op_jeq)
/**
 * @brief       Represents 'jne' opcode (jump if not equal).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the comparison flag (CF) is NOT zero.
 */
cloxDefineOpCode(CLOX_OP_CODE_JNE,      0x14,   "jne",      CLOX_OP_KIND_JUMP,  _op_jne)
/**
 * @brief       Represents 'jgt' opcode (jump if greater than).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the comparison flag (CF) is two.
 */
cloxDefineOpCode(CLOX_OP_CODE_JGT,      0x15,   "jgt",      CLOX_OP_KIND_JUMP,  _op_jgt)
/**
 * @brief       Represents 'jge' opcode (jump if greater or equal).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the comparison flag (CF) is two or zero, so if it's even.
 */
cloxDefineOpCode(CLOX_OP_CODE_JGE,      0x16,   "jge",      CLOX_OP_KIND_JUMP,  _op_jge)
/**
 * @brief       Represents 'jlt' opcode (jump if less than).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the comparison flag (CF) is one.
 */
cloxDefineOpCode(CLOX_OP_CODE_JLT,      0x17,   "jlt",      CLOX_OP_KIND_JUMP,  _op_jlt)
/**
 * @brief       Represents 'jle' opcode (jump if less or equal).
 * 
 * @note        This opcode 'jumps' an amount of bytes specified by the argument
 *              adding its value (as an offset) to the program counter if the
 *              value of the comparison flag (CF) is one or zero, so if it's less
 *              than two.
 */
cloxDefineOpCode(CLOX_OP_CODE_JLE,      0x18,   "jle",      CLOX_OP_KIND_JUMP,  _op_jle)
/**
 * @brief       Represents 'br' opcode (branch).
 * 
 * @note        This opcode 'jumps' to a specific label or address in the source
 *              bytecode, modifing the value of the program counter.
 */
cloxDefineOpCode(CLOX_OP_CODE_BR,       0x19,   "br",       CLOX_OP_KIND_JUMP,  _op_br)
/**
 * @brief       Represents 'beq' opcode (branch if equal).
 * 
 * @note        This opcode 'jumps' to a specific label or address in the source
 *              bytecode, modifing the value of the program counter if the value
 *              of the comparison flag (CF) is zero.
 */
cloxDefineOpCode(CLOX_OP_CODE_BEQ,      0x1A,   "beq",      CLOX_OP_KIND_JUMP,  _op_beq)
/**
 * @brief       Represents 'bnq' opcode (branch if not equal).
 * 
 * @note        This opcode 'jumps' to a specific label or address in the source
 *              bytecode, modifing the value of the program counter if the value
 *              of the comparison flag (CF) is NOT zero.
 */
cloxDefineOpCode(CLOX_OP_CODE_BNE,      0x1B,   "bne",      CLOX_OP_KIND_JUMP,  _op_bne)
/**
 * @brief       Represents 'br' opcode (branch).
 * 
 * @note        This opcode 'jumps' to a specific label or address in the source
 *              bytecode, modifing the value of the program counter.
 */
cloxDefineOpCode(CLOX_OP_CODE_BGT,      0x1C,   "bgt",      CLOX_OP_KIND_JUMP,  _op_bgt)
/**
 * @brief       Represents 'br' opcode (branch).
 * 
 * @note        This opcode 'jumps' to a specific label or address in the source
 *              bytecode, modifing the value of the program counter.
 */
cloxDefineOpCode(CLOX_OP_CODE_BGE,      0x1D,   "bge",      CLOX_OP_KIND_JUMP,  _op_bge)
/**
 * @brief       Represents 'br' opcode (branch).
 * 
 * @note        This opcode 'jumps' to a specific label or address in the source
 *              bytecode, modifing the value of the program counter.
 */
cloxDefineOpCode(CLOX_OP_CODE_BLT,      0x1E,   "blt",      CLOX_OP_KIND_JUMP,  _op_blt)
/**
 * @brief       Represents 'br' opcode (branch).
 * 
 * @note        This opcode 'jumps' to a specific label or address in the source
 *              bytecode, modifing the value of the program counter.
 */
cloxDefineOpCode(CLOX_OP_CODE_BLE,      0x1F,   "ble",      CLOX_OP_KIND_JUMP,  _op_ble)

/* =---- Data Transfer OpCodes ---------------------------------= */

/**
 * Move OpCodes
 */

/**
 * @brief       Represents 'mov' opcode (move).
 * 
 * @note        This opcode moves to a register the content of another register
 *              or a value from the evaluation stack.
 */
cloxDefineOpCode(CLOX_OP_CODE_MOV,      0x20,   "mov",      CLOX_OP_KIND_DATA,  _op_mov)

/**
 * Load OpCodes
 */

/**
 * @brief       Represents 'ldc' opcode (load constant).
 * 
 * @note        This opcode loads into the specified register a constant value
 *              directly specified.
 */
cloxDefineOpCode(CLOX_OP_CODE_LDC,      0x24,   "ldc",      CLOX_OP_KIND_DATA,  _op_ldc)
/**
 * @brief       Represents 'lda' opcode (load address).
 * 
 * @note        This opcode loads into the specified register the address there
 *              directly specified.
 */
cloxDefineOpCode(CLOX_OP_CODE_LDA,      0x25,   "lda",      CLOX_OP_KIND_DATA,  _op_lda)

/**
 * @brief       Represents 'lec' opcode (load effective constant).
 * 
 * @note        This opcode loads into the specified register a constant value
 *              from constants pool.
 */
cloxDefineOpCode(CLOX_OP_CODE_LEC,      0x26,   "lec",      CLOX_OP_KIND_DATA,  _op_lec)
/**
 * @brief       Represents 'lea' opcode (load effective address).
 * 
 * @note        This opcode loads into the specified register the address of a
 *              value in constants pool.
 */
cloxDefineOpCode(CLOX_OP_CODE_LEA,      0x27,   "lea",      CLOX_OP_KIND_DATA,  _op_lea)

/* =------------------------------------------------------------= */

/**
 * @}
 */

#undef cloxDefineOpCode
