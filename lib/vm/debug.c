/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/errno.h"
#include "clox/base/file.h"
#include "clox/vm/debug.h"
#include "clox/vm/code.h"

#ifndef CLOX_DISASSEMBLER_OFFSET_FORMAT
#   if CLOX_ARCHTECT_IS_64_BIT
#       define CLOX_DISASSEMBLER_OFFSET_FORMAT "%08X"
#   else
#       define CLOX_DISASSEMBLER_OFFSET_FORMAT "%04X"
#   endif
#endif

CLOX_API void CLOX_STDCALL cloxDisassembleInstruction(FILE *const stream, CloxCodeBlockReader_t *const codeBlockReader)
{
    CloxOpCodeInfo_t opCodeInfo;

    if (cloxGetOpCodeInfo(cloxCodeBlockReaderGet(codeBlockReader), &opCodeInfo))
    {
        fprintf(stream, CLOX_DISASSEMBLER_OFFSET_FORMAT " %-8s", (uint32_t)codeBlockReader->index - 1, opCodeInfo.name);

        switch (opCodeInfo.kind)
        {
        default:
            unreach();
        }
    }
    else
    {
        fprintf(stream, CLOX_DISASSEMBLER_OFFSET_FORMAT " uknown (%02X)", (uint32_t)codeBlockReader->index - 1, opCodeInfo.code);
    }

    fputc(EOL, stream);

    return;
}

CLOX_API void CLOX_STDCALL cloxDisassembleCodeBlock(FILE *const stream, const CloxCodeBlock_t *const codeBlock)
{
    assert(stream != NULL);

    CloxCodeBlockReader_t codeBlockReader;

    if (cloxInitCodeBlockReader(&codeBlockReader, codeBlock)->array)
    {
        while (!cloxCodeBlockReaderIsAtEnd(&codeBlockReader))
            cloxDisassembleInstruction(stream, &codeBlockReader);
    }

    return;
}
