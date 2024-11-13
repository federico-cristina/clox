#pragma once

/**
 * @file        opcode.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 * 
 * @brief       In this header are defined operation bytecodes and functions
 *              to validate them and to deal with.
 */

#ifndef CLOX_VM_OPCODE_H_
#define CLOX_VM_OPCODE_H_

#include "clox/base/api.h"
#include "clox/base/bool.h"

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    OPMODES OpModes
 * @{
 */

#pragma region OpModes

/**
 * @brief       This enumeration provides opcode arguments taking modalities
 *              used to capture the correct number of bytcode arguments.
 */
typedef enum _CloxOpMode
{
    /**
     * @brief   Represents an opcode that doesn't needs additional bytecode
     *          arguments.
     */
    CLOX_OP_MODE_NONE,
    /**
     * @brief   Represents an opcode that takes only the following bytecode
     *          as argument.
     */
    CLOX_OP_MODE_BYTE,
    /**
     * @brief   Represents an opcode that takes the three following bytes (a
     *          24-bit integer) as argument.
     */
    CLOX_OP_MODE_LONG,
    /**
     * @brief   Represents an opcode that takes an amount of bytes as argument
     *          as specified by the following byte.
     */
    CLOX_OP_MODE_READ,
    /**
     * @brief   Represents an opcode that takes a variable sequence of bytes
     *          as argument: while the byte taken as argument has the BYTE_MAX
     *          value, the argument decoder, takes the next and repeat the
     *          operation.
     * 
     * @example In the following bytecode 7F FF F3 F1 where 7F is an opcode with
     *          CLOX_OP_MODE_NEXT flag, arguments taken are FF F3, but not F1.
     */
    CLOX_OP_MODE_NEXT,
    /**
     * @brief   Represents an opcode that takes a variable number of arguments
     *          specified by a type code.
     */
    CLOX_OP_MODE_SCAN,
} CloxOpMode_t;

#pragma endregion

/**
 * @}
 * 
 * @defgroup    OPCODES OpCodes
 * @{
 */

#pragma region OpCodes

#ifndef CLOX_VM_OPCODE_INC_
/**
 * @brief       This constant represents the inclusion path to 'opcode.inc'
 *              x-macro file.
 */
#   define CLOX_VM_OPCODE_INC_ "clox/vm/opcode.inc"
#endif

/**
 * @brief       This enumeration provides instruction opcodes used to choose
 *              which operation execute.
 */
typedef enum _CloxOpCode
{
    /**
     * @brief   Represents 'nop' opcode (no operation).
     */
    CLOX_OP_CODE_NOP = 0x00,

#ifndef cloxDefineOpCode
#   if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This macro defines an opcode specifing also its properties,
 *              used in CLOX_VM_OPCODES_INC_ file.
 */
#       define cloxDefineOpCode(opEnum, opCode, opName, opMode) opEnum = opCode,
#   else
/**
 * @brief       This macro defines an opcode specifing also its properties,
 *              used in CLOX_VM_OPCODES_INC_ file.
 */
#       define cloxDefineOpCode(opEnum, opCode, opName, opMode) opEnum,
#   endif
#endif

#include CLOX_VM_OPCODE_INC_

#ifdef cloxDefineOpCode
#   undef cloxDefineOpCode
#endif
} CloxOpCode_t;

/**
 * @brief       This data structure provides a container to store opcodes
 *              informations like name or argument taking modality.
 */
typedef struct _CloxOpCodeInfo
{
    /**
     * @brief   Represents the displayable name of the opcode.
     */
    const char  *name;
    /**
     * @brief   Represents the numeric value of the opcode.
     */
    CloxOpCode_t code;
    /**
     * @brief   Represents the argument taking modality.
     */
    CloxOpMode_t mode;
} CloxOpCodeInfo_t;

/**
 * @brief       This function looks up for selected opcode informations
 *              and fills up outOpCodeInfo with them.
 * 
 * @param       opCode The opcode to find and of which are desired the
 *              informations.
 * @param       outOpCodeInfo A pointer to a CloxOpCodeInfo_t instance in
 *              which will be written the result.
 * @return      On success (so if the opcode is found) this function returns
 *              TRUE and sets the value of outOpCodeInfo parameter to the
 *              values specified by the opCode parameter. On failure FALSE
 *              is returned and outOpCodeInfo parameter is filled with error
 *              informations.
 */
CLOX_API bool_t CLOX_STDCALL cloxGetOpCodeInfo(const CloxOpCode_t opCode, CloxOpCodeInfo_t *const outOpCodeInfo);

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_VM_OPCODE_H_ */
