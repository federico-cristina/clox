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

#include "clox/base/api.h"
#include "clox/base/bool.h"

CLOX_C_HEADER_BEGIN

/**
 * @brief       Enumerates virtual machine's opcode arguments-taking mode.
 */
typedef enum _CloxOpMode
{
    /**
     * @brief   OpCodes that have no arguments.
     */
    CLOX_OP_MODE_NONE,
    /**
     * @brief   OpCodes that takes a single byte as argument.
     */
    CLOX_OP_MODE_BYTE,
    /**
     * @brief   OpCodes that takes multiple bytes as argument, scanning them
     *          from the bytecode.
     */
    CLOX_OP_MODE_SCAN,
} CloxOpMode_t;

#ifndef CLOX_VM_OPCODES_INC_
/**
 * @brief       This constant represents the inclusion path to 'opcode.inc'
 *              x-macro file.
 */
#   define CLOX_VM_OPCODES_INC_ "clox/vm/opcode.inc"
#endif

/**
 * @brief       Enumerates virtual machine's opcodes. 
 */
typedef enum _CloxOpCode
{
    /**
     * @brief   No Operation. (nop)
     */
    CLOX_OP_CODE_NOP = 0,

#ifndef cloxDefineOpCode
#   define cloxDefineOpCode(opCode, opName, opMode) opCode,
#endif

#include CLOX_VM_OPCODES_INC_

#ifdef cloxDefineOpCode
#   undef cloxDefineOpCode
#endif
} CloxOpCode_t;

/**
 * @brief       OpCode informations data structure.
 */
typedef struct _CloxOpCodeInfo
{
    /**
     * @brief   A pointer to a constant string representing the operation
     *          display name.
     */
    const char  *name;
    /**
     * @brief   An integer representing the operation code (opcode).
     */
    CloxOpCode_t code;
    /**
     * @brief   An integer representing the arguments-taking modality of
     *          the operation.
     */
    CloxOpMode_t mode;
} CloxOpCodeInfo_t;

/**
 * @brief       This function checks the validity of the specified opcode
 *              and, when outOpCodeInfo parameter is not NULL sets its fields
 *              with the opcode informations.
 * 
 * @param       opCode The opcode to check.
 * @param       outOpCodeInfo A pointer to a CloxOpCodeInfo_t struct to store
 *              opcode informations.
 * @return      TRUE when the opcode is valid, FALSE instead.
 */
CLOX_API bool_t CLOX_STDCALL cloxGetOpCodeInfo(CloxOpCode_t opCode, CloxOpCodeInfo_t *const outOpCodeInfo);

CLOX_C_HEADER_END

#endif /* CLOX_VM_OPCODES_H_ */
