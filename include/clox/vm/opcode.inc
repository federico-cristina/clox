/**                                                                     -*- C -*-
 * @file        opcode.inc
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
#   define cloxDefineOpCode(opEnum, opCode, opName, opMode)
#endif

/**
 * @defgroup    OPCODES OpCodes
 * @{
 */

/* =---- Status Control OpCodes --------------------------------= */

/**
 * @brief       Represents 'break' opcode (breakpoint).
 */
cloxDefineOpCode(CLOX_OP_CODE_BREAK,    0x01,   "break",    CLOX_OP_MODE_NONE)
/**
 * @brief       Represents 'abort' opcode (abort).
 */
cloxDefineOpCode(CLOX_OP_CODE_ABORT,    0x02,   "abort",    CLOX_OP_MODE_NONE)
/**
 * @brief       Represents 'exit' opcode (exit).
 */
cloxDefineOpCode(CLOX_OP_CODE_EXIT,     0x03,   "exit",     CLOX_OP_MODE_BYTE)
/**
 * @brief       Represents 'raise' opcode (raise).
 */
cloxDefineOpCode(CLOX_OP_CODE_RAISE,    0x04,   "raise",    CLOX_OP_MODE_BYTE)

/* =---- Loading OpCodes ---------------------------------------= */

/**
 * @brief       Represents 'ldb' opcode (load byte).
 * 
 * @note        This opcode loads onto the evaluation stack a byte specified by
 *              the argument.
 */
cloxDefineOpCode(CLOX_OP_CODE_LDB,      0x20,   "ldb",      CLOX_OP_MODE_BYTE)
/**
 * @brief       Represents 'ldl' opcode (load long).
 * 
 * @note        This opcode loads onto the evaluation stack a value specified by
 *              the long argument.
 */
cloxDefineOpCode(CLOX_OP_CODE_LDL,      0x21,   "ldl",      CLOX_OP_MODE_LONG)
/**
 * @brief       Represents 'ldn' opcode (load n-bytes).
 * 
 * @note        This opcode loads onto the evaluation stack a sequence of n-bytes,
 *              where n is the first byte after the opcode.
 */
cloxDefineOpCode(CLOX_OP_CODE_LDN,      0x22,   "ldn",      CLOX_OP_MODE_READ)
/**
 * @brief       Represents 'lds' opcode (load sequence).
 * 
 * @note        This opcode loads onto the evaluation stack an arbitrary sequence
 *              of chained bytes.
 */
cloxDefineOpCode(CLOX_OP_CODE_LDS,      0x23,   "lds",      CLOX_OP_MODE_NEXT)
/**
 * @brief       Represents 'ldc' opcode (load constant).
 * 
 * @note        This opcode loads onto the evaluation stack a constant value
 *              specified by the scanned argument.
 */
cloxDefineOpCode(CLOX_OP_CODE_LDC,      0x24,   "ldc",      CLOX_OP_MODE_SCAN)

/**
 * @brief       Represents 'lec' opcode (load effective constant).
 * 
 * @note        This opcode loads onto the evaluation stack the constant value
 *              stored into the constants data table filled by the virtual
 *              machine during compilation and interpretation at the index
 *              specified by the opcode argument.
 */
cloxDefineOpCode(CLOX_OP_CODE_LEC,      0x25,   "lec",      CLOX_OP_MODE_LONG)
/**
 * @brief       Represents 'lea' opcode (load effective address).
 * 
 * @note        This opcode loads onto the evaluation stack the address of the
 *              entry of the constants data table filled by the virtual machine
 *              during compilation and interpretation at the index specified by
 *              the opcode argument.
 */
cloxDefineOpCode(CLOX_OP_CODE_LEA,      0x26,   "lea",      CLOX_OP_MODE_LONG)

/* =------------------------------------------------------------= */

#undef cloxDefineOpCode

/**
 * @}
 */
