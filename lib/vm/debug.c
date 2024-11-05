/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/errno.h"
#include "clox/base/file.h"

#include "clox/vm/debug.h"
#include "clox/vm/opcode.h"

CLOX_INLINE void CLOX_STDCALL cloxDisassembleInstruction(FILE *const stream, CloxChunkReader_t *const chunkReader)
{
    CLOX_REGISTER CloxOpCode_t opCode = cloxChunkReaderGet(chunkReader);
    CloxOpCodeInfo_t opCodeInfo;

    if (cloxGetOpCodeInfo(opCode, &opCodeInfo))
    {
        fprintf(stream, "%08X %-16s", (uint32_t)chunkReader->index - 1, opCodeInfo.name);

        switch (opCodeInfo.mode)
        {
        case CLOX_OP_MODE_NONE:
            break;

        case CLOX_OP_MODE_BYTE:
            fprintf(stream, " %02X", cloxChunkReaderGet(chunkReader));
            break;

        case CLOX_OP_MODE_SCAN:
            notimpl();
        
        default:
            unreach();
        }
    }
    else
    {
        fprintf(stream, "%08X uknown (%02X)", (uint32_t)chunkReader->index - 1, opCodeInfo.code);
    }

    fputc(EOL, stream);

    return;
}

CLOX_API void CLOX_STDCALL cloxDisassembleChunk(FILE *const stream, const CloxChunk_t *const chunk)
{
    assert(stream != NULL);

    CloxChunkReader_t chunkReader;

    if (cloxInitChunkReader(&chunkReader, chunk)->array)
    {
        while (!cloxChunkReaderIsAtEnd(&chunkReader))
            fputs("  ", stream), cloxDisassembleInstruction(stream, &chunkReader);
    }

    return;
}
