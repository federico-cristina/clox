/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/errno.h"

#include "clox/vm/opcodes.h"

CLOX_API const char *CLOX_STDCALL cloxGetOpCodeName(CloxOpCode_t opCode)
{
    switch (opCode)
    {
    case CLOX_OPCODE_NOP:
        return "nop";

#ifndef cloxDefineOpCode
#   define cloxDefineOpCode(name, opCode) \
    case name:							  \
        return opCode;
#endif

#include CLOX_VM_OPCODES_INC_

#ifdef cloxDefineOpCode
#   undef cloxDefineOpCode
#endif

    default:
        return unreach();
    }
}
