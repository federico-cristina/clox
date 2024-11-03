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
#   define cloxDefineOpCode(name, opCode)
#endif

/**
 * @brief       Return opcode. (ret)
 */
cloxDefineOpCode(CLOX_OPCODE_RET,       "ret")
