#pragma once

/**
 * @file        debug.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 * 
 * @brief       In this header are defined tool function to get debugging
 *              informations from the virtual machine and from the bytecode.
 */

#ifndef CLOX_VM_DEBUG_H_
#define CLOX_VM_DEBUG_H_

#include "clox/vm/code_block.h"

#include <stdio.h>

#ifndef cloxDebug
#   if CLOX_DEBUG
/**
 * @brief       This macro provides conditional debugging statements compilation.
 */
#       define cloxDebug(statement) statement
#   else
/**
 * @brief       This macro provides conditional debugging statements compilation.
 */
#       define cloxDebug(statement)
#   endif
#endif

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    DEBUGGER_DISASSEMBLER Debugger Disassembler
 * @{
 */

#pragma region Debugger Disassembler

/**
 * @brief       This function disassembles the bytecode stored into a specific
 *              CloxCodeBlock_t instance, printing disassembled data into a
 *              FILE stream.
 * 
 * @param       stream A pointer to the FILE stream handler to which print the
 *              disassembled data.
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance to from which
 *              disassemble bytecode.
 */
CLOX_API void CLOX_STDCALL cloxDisassembleCodeBlock(FILE *const stream, const CloxCodeBlock_t *const codeBlock);

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_VM_DEBUG_H_ */
