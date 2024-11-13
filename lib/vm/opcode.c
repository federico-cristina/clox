/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/bits.h"
#include "clox/base/byte.h"
#include "clox/vm/opcode.h"

#ifndef CLOX_OP_CODE_INFOS_MAX
#   ifndef CLOX_OPTIMIZE
#       define CLOX_OP_CODE_INFOS_MAX (BYTE_MAX + 1)
#   else
#       define CLOX_OP_CODE_INFOS_MAX
#   endif
#endif

CLOX_STATIC const CloxOpCodeInfo_t clox_OpCodeInfos[CLOX_OP_CODE_INFOS_MAX] = {
#if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
    [CLOX_OP_CODE_NOP] = {
        .name = "nop",
        .code = CLOX_OP_CODE_NOP,
        .mode = CLOX_OP_MODE_NONE,
    },

#   ifndef cloxDefineOpCode
#      define cloxDefineOpCode(opEnum, opCode, opName, opMode) \
    [opCode] = {                                               \
        .name = opName,                                        \
        .code = opEnum,                                        \
        .mode = opMode,                                        \
    },
#   endif

#   include CLOX_VM_OPCODE_INC_

#   ifdef cloxDefineOpCode
#       undef cloxDefineOpCode
#   endif
#else
    {
        .name = "nop",
        .code = CLOX_OP_CODE_NOP,
        .mode = CLOX_OP_MODE_NONE,
    },

#   ifndef cloxDefineOpCode
#      define cloxDefineOpCode(opEnum, opCode, opName, opMode) \
    {                                                          \
        .name = opName,                                        \
        .code = opEnum,                                        \
        .mode = opMode,                                        \
    },
#   endif

#   include CLOX_VM_OPCODE_INC_

#   ifdef cloxDefineOpCode
#       undef cloxDefineOpCode
#   endif
#endif
};

CLOX_API bool_t CLOX_STDCALL cloxGetOpCodeInfo(const CloxOpCode_t opCode, CloxOpCodeInfo_t *const outOpCodeInfo)
{
    CLOX_REGISTER bool_t result;

    if ((opCode < 0) || (opCode >= countof(clox_OpCodeInfos)))
    {
        if (outOpCodeInfo)
        {
            outOpCodeInfo->name = "uknown";
            outOpCodeInfo->code = opCode;
            outOpCodeInfo->mode = CLOX_OP_MODE_NONE;
        }

        result = FALSE;
    }
    else
    {
        if (outOpCodeInfo)
        {
            const CloxOpCodeInfo_t *const opCodeInfo = &clox_OpCodeInfos[opCode];

            outOpCodeInfo->name = opCodeInfo->name;
            outOpCodeInfo->code = opCodeInfo->code;
            outOpCodeInfo->mode = opCodeInfo->mode;
        }

        result = TRUE;
    }

    return result;
}
