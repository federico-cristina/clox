#pragma once

/**
 * @brief 
 * 
 */

#ifndef CLOX_VM_CODE_H_
#define CLOX_VM_CODE_H_

#include "clox/base/api.h"
#include "clox/base/bits.h"
#include "clox/base/bool.h"

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    OP_KINDS OpKinds
 * @{
 */

#pragma region OpKinds

typedef enum _CloxOpKind
{
    /**
     * @brief   This kind of opcodes denotes instructions that don't
     *          need any kind of extra byte to be processed.
     * 
     * @note    Length: 8-bit
     * @note    Layout: O
     */
    CLOX_OP_KIND_BYTE = 0x0101,
    /**
     * @brief   This kind of opcodes denotes instructions that uses a
     *          signle byte as extra argument, generally are used in
     *          FAST operations.
     * 
     *          FAST operations are instructions with a reduced number
     *          of arguments.
     * 
     * @note    Length: 16-bit
     * @note    Layout: O  bX
     */
    CLOX_OP_KIND_FAST = 0x0202,
    /**
     * @brief   This kind of opcodes denotes instructions that requires
     *          only a flagged argument.
     * 
     * @note    Length: 32-bit
     * @note    Layout: O  hX  F
     */
    CLOX_OP_KIND_CTRL = 0x0403,
    /**
     * @brief   This kind of opcodes denotes instructions that transfer
     *          some data from an addressable location to a fast-access
     *          register.
     * 
     * @note    Length: 32-bit
     * @note    Layout: ObZhX
     */
    CLOX_OP_KIND_DATA = 0x0404,
    /**
     * @brief   This kind of opcodes denotes instructions that performs
     *          some kind of operation between registers, storing the result
     *          into another one.
     *
     * @note    Length: 32-bit
     * @note    Layout: ObZbXbY
     */
    CLOX_OP_KIND_REGS = 0x0405,
    /**
     * @brief   This kind of opcodes denotes instructions that performs
     *          some kind of operation between two addressable locations,
     *          storing the result into a register.
     * 
     * @note    Length: 48-bit
     * @note    Layout: ObZhXhY
     */
    CLOX_OP_KIND_LONG = 0x0606,
    /**
     * @brief   This kind of opcodes denotes instructions that performs
     *          some kind of branching (jump) operation.
     * 
     * @note    Length: 48-bit
     * @note    Layout: OdZ    F
     */
    CLOX_OP_KIND_JUMP = 0x0607,
    /**
     * @brief   This kind of opcodes denotes instructions that uses each
     *          possible argument.
     * 
     * @note    Length: 64-bit
     * @note    Layout: OhZhXhYF
     */
    CLOX_OP_KIND_FULL = 0x0808,
} CloxOpKind_t;

#ifndef cloxGetOpKindSize
#   define cloxGetOpKindSize(opKind) ((opKind) >> CHAR_BIT)
#endif

#pragma endregion

/**
 * @}
 * 
 * @defgroup    OP_CODES OpCodes
 * @{
 */

#ifndef CLOX_VM_OPCODE_INC_
/**
 * @brief       This constant represents the inclusion path to 'code.inc'
 *              x-macro file.
 */
#   define CLOX_VM_OPCODE_INC_ "clox/vm/code.inc"
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
#       define cloxDefineOpCode(opEnum, opCode, ...) opEnum = opCode,
#   else
/**
 * @brief       This macro defines an opcode specifing also its properties,
 *              used in CLOX_VM_OPCODES_INC_ file.
 */
#       define cloxDefineOpCode(opEnum, opCode, ...) opEnum,
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
     * @brief   Represents the instruction handler address.
     */
    void       (*func)(void);
    /**
     * @brief   Represents the numeric value of the opcode.
     */
    CloxOpCode_t code;
    /**
     * @brief   Represents the argument taking modality.
     */
    CloxOpKind_t kind;
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

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_VM_CODE_H_ */
