#pragma once

/**
 * @file        opcodes.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 */

#ifndef CLOX_VM_OPCODES_H_
#define CLOX_VM_OPCODES_H_

#include "clox/base/byte.h"

CLOX_C_HEADER_BEGIN

#ifndef CLOX_VM_OPCODES_INC_
/**
 * @brief       This constant represents the inclusion path to 'opcode.inc'
 *              x-macro file.
 */
#   define CLOX_VM_OPCODES_INC_ "clox/vm/opcodes.inc"
#endif

/**
 * @brief       Enumerates virtual machine's opcodes. 
 */
typedef enum _CloxOpCode
{
    /**
     * @brief   No Operation. (nop)
     */
    CLOX_OPCODE_NOP = 0,

#ifndef cloxDefineOpCode
#   define cloxDefineOpCode(name) name,
#endif

#include CLOX_VM_OPCODES_INC_

#ifdef cloxDefineOpCode
#   undef cloxDefineOpCode
#endif
} CloxOpCode_t;

CLOX_C_HEADER_END

#endif /* CLOX_VM_OPCODES_H_ */
