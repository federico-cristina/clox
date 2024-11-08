/**                                                                     -*- C -*-
 * @file        opcodes.inc
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
#   define cloxDefineOpCode(opCode, opName, opMode)
#endif

/**
 * @brief       Return opcode. (ret)
 */
cloxDefineOpCode(CLOX_OP_CODE_RET,      "ret",      CLOX_OP_MODE_NONE)

/**
 * @brief       Load Byte opcode, loads a single byte onto the evaluation
 *              stack. (ldbyte)
 */
cloxDefineOpCode(CLOX_OP_CODE_LDBYTE,   "ldbyte",   CLOX_OP_MODE_BYTE)
