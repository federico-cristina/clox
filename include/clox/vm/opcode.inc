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

/* =---- Status Control OpCodes --------------------------------= */

/**
 * @brief       Represents 'break' opcode (breakpoint).
 */
cloxDefineOpCode(CLOX_OP_CODE_BREAK,    0x01,   "break",    CLOX_OP_MODE_NONE)
/**
 * @brief       Represents 'abort' opcode (aborts execution).
 */
cloxDefineOpCode(CLOX_OP_CODE_ABORT,    0x02,   "abort",    CLOX_OP_MODE_NONE)
/**
 * @brief       Represents 'exit' opcode (exits execution with an exit code).
 */
cloxDefineOpCode(CLOX_OP_CODE_EXIT,     0x03,   "exit",     CLOX_OP_MODE_BYTE)
/**
 * @brief       Represents 'raise' opcode (raises a signal).
 */
cloxDefineOpCode(CLOX_OP_CODE_RAISE,    0x04,   "raise",    CLOX_OP_MODE_BYTE)

/* =------------------------------------------------------------= */

/**
 * @brief       Represents 'ret' opcode (return).
 */
cloxDefineOpCode(CLOX_OP_CODE_RET, 0xF0,      "ret",      CLOX_OP_MODE_NONE)

/**
 * @brief       Represents 'ldbyte' opcode (loada a byte).
 */
cloxDefineOpCode(CLOX_OP_CODE_LDBYTE, 0xF1,   "ldbyte",   CLOX_OP_MODE_BYTE)
