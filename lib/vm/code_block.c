/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/alloc.h"
#include "clox/vm/code_block.h"

#ifndef CLOX_CODE_BLOCK_GROWING_FACTOR
#   define CLOX_CODE_BLOCK_GROWING_FACTOR 2
#endif

CLOX_API CloxCodeBlock_t *CLOX_STDCALL cloxInitCodeBlock(CloxCodeBlock_t *const codeBlock, size_t capacity)
{
    assert(codeBlock != NULL);

    if (capacity)
    {
        capacity = cloxAlignToWordPtr(capacity);

        codeBlock->array = dim(byte_t, capacity);
        codeBlock->count = 0;
        codeBlock->capacity = capacity;
    }
    else
    {
        codeBlock->array = NULL;
        codeBlock->count = 0;
        codeBlock->capacity = 0;
    }

    return codeBlock;
}

CLOX_API CloxCodeBlock_t *CLOX_STDCALL cloxFreeCodeBlock(CloxCodeBlock_t *const codeBlock)
{
    assert(codeBlock != NULL);

    if (codeBlock->capacity)
        free(codeBlock->array);
    
    codeBlock->array = NULL;
    codeBlock->count = 0;
    codeBlock->capacity = 0;

    return codeBlock;
}

CLOX_API CloxCodeBlock_t *CLOX_STDCALL cloxCreateCodeBlock(const size_t capacity)
{
    return cloxInitCodeBlock(alloc(CloxCodeBlock_t), capacity);
}

CLOX_API void CLOX_STDCALL cloxCodeBlockResize(CloxCodeBlock_t *const codeBlock, size_t newCapacity)
{
    assert(codeBlock != NULL);

    if (codeBlock->capacity)
    {
        if (newCapacity)
        {
            newCapacity = cloxAlignToWordPtr(newCapacity);

            codeBlock->array = redim(byte_t, codeBlock->array, newCapacity);

            if (codeBlock->count >= newCapacity)
                codeBlock->count = newCapacity - 1;

            codeBlock->capacity = newCapacity;
        }
        else
        {
            cloxFreeCodeBlock(codeBlock);
        }
    }
    else
    {
        cloxInitCodeBlock(codeBlock, newCapacity);
    }

    return;
}

CLOX_API void CLOX_STDCALL cloxCodeBlockExpand(CloxCodeBlock_t *const codeBlock, const uint32_t offset)
{
    return cloxCodeBlockResize(codeBlock, codeBlock->capacity + offset);
}

CLOX_API void CLOX_STDCALL cloxCodeBlockShrink(CloxCodeBlock_t *const codeBlock, const uint32_t offset)
{
    return cloxCodeBlockResize(codeBlock, codeBlock->capacity - offset);
}

CLOX_INLINE void CLOX_STDCALL clox_CodeBlockGrow(CloxCodeBlock_t *const codeBlock)
{
    return cloxCodeBlockResize(codeBlock, codeBlock->capacity * CLOX_CODE_BLOCK_GROWING_FACTOR);
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockPush(CloxCodeBlock_t *const codeBlock, const byte_t value)
{
    assert(codeBlock != NULL);

    if (codeBlock->count >= codeBlock->capacity)
        clox_CodeBlockGrow(codeBlock);

    return codeBlock->array[codeBlock->count++] = value;
}

CLOX_INLINE bool_t CLOX_STDCALL clox_CodeBlockCheckBounds(const CloxCodeBlock_t *const codeBlock, const size_t index)
{
    return (index > 0) && (index < codeBlock->capacity);
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockPeek(const CloxCodeBlock_t *const codeBlock, const uint32_t offset)
{
    assert(codeBlock != NULL);

    CLOX_REGISTER byte_t result;

    if (clox_CodeBlockCheckBounds(codeBlock, codeBlock->count - offset))
        result = codeBlock->array[codeBlock->count - offset - 1];
    else
        fail(CLOX_ERROR_MESSAGE_INDEX_OUT_OF_BOUNDS, NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockPop(CloxCodeBlock_t *const codeBlock)
{
    assert(codeBlock != NULL);

    CLOX_REGISTER byte_t result;

    if (codeBlock->count > 0)
        result = codeBlock->array[--codeBlock->count];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_UNDERRUN, NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockTop(const CloxCodeBlock_t *const codeBlock)
{
    assert(codeBlock != NULL);

    CLOX_REGISTER byte_t result;

    if (codeBlock->count > 0)
        result = codeBlock->array[codeBlock->count - 1];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_UNDERRUN, NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockGet(const CloxCodeBlock_t *const codeBlock, const size_t index)
{
    assert(codeBlock != NULL);

    CLOX_REGISTER byte_t result;

    if (clox_CodeBlockCheckBounds(codeBlock, index))
        result = codeBlock->array[index];
    else
        fail(CLOX_ERROR_MESSAGE_INDEX_OUT_OF_BOUNDS, NULL);

    return result;
}

CLOX_API const byte_t *CLOX_STDCALL cloxCodeBlockWrite(CloxCodeBlock_t *const codeBlock, const byte_t *const buffer, const size_t count)
{
    assert(codeBlock != NULL);

    if ((codeBlock->count + count) >= codeBlock->capacity)
        cloxCodeBlockExpand(codeBlock, (codeBlock->count + count) - codeBlock->capacity);

    bufcpy(codeBlock->array, buffer, count);

    return buffer;
}

CLOX_API void CLOX_STDCALL cloxDeleteCodeBlock(CloxCodeBlock_t *const codeBlock)
{
    assert(codeBlock != NULL);

    if (codeBlock->array)
        free(codeBlock->array);

    free(codeBlock);
    
    return;
}

CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxInitCodeBlockReader(CloxCodeBlockReader_t *const codeBlockReader, const CloxCodeBlock_t *const codeBlock)
{
    assert(codeBlockReader != NULL && codeBlock != NULL);

    codeBlockReader->array = codeBlock->array;
    codeBlockReader->count = codeBlock->count;
    codeBlockReader->index = 0;

    return codeBlockReader;
}

CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxInitCodeBlockReaderFromBuffer(CloxCodeBlockReader_t *const codeBlockReader, const byte_t *const buffer, const size_t count)
{
    assert(codeBlockReader != NULL && buffer != NULL);

    codeBlockReader->array = buffer;
    codeBlockReader->count = count;
    codeBlockReader->index = 0;

    return codeBlockReader;
}

CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxFreeCodeBlockReader(CloxCodeBlockReader_t *const codeBlockReader, const bool_t freeArray)
{
    assert(codeBlockReader != NULL);

    if (freeArray && codeBlockReader->count)
        free(codeBlockReader->array);

    codeBlockReader->array = NULL;
    codeBlockReader->count = 0;
    codeBlockReader->index = 0;

    return codeBlockReader;
}

CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxCreateCodeBlockReader(const CloxCodeBlock_t *const codeBlock)
{
    return cloxInitCodeBlockReader(alloc(CloxCodeBlockReader_t), codeBlock);
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockReaderGet(CloxCodeBlockReader_t *const codeBlockReader)
{
    assert(codeBlockReader != NULL);

    CLOX_REGISTER byte_t result;

    if (codeBlockReader->index < codeBlockReader->count)
        result = codeBlockReader->array[codeBlockReader->index++];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_OVERRUN, NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockReaderTop(const CloxCodeBlockReader_t *const codeBlockReader)
{
    assert(codeBlockReader != NULL);

    CLOX_REGISTER byte_t result;

    if (codeBlockReader->index < codeBlockReader->count)
        result = codeBlockReader->array[codeBlockReader->index];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_OVERRUN, NULL);

    return result;
}

CLOX_API size_t CLOX_STDCALL cloxCodeBlockReaderRead(CloxCodeBlockReader_t *const codeBlockReader, byte_t *const outBuffer, const size_t count)
{
    assert(codeBlockReader != NULL);

    CLOX_REGISTER size_t readCount, i, n;

    if (outBuffer)
    {
        i = codeBlockReader->index;
        n = codeBlockReader->count;

        CLOX_REGISTER byte_t *array = codeBlockReader->array;

        for (readCount = 0; (readCount < count) && (i < n); readCount++, i++)
            outBuffer[readCount] = array[i + readCount];

        codeBlockReader->index = i;
    }
    else
    {
        readCount = 0;
    }

    return readCount;
}

CLOX_API byte_t CLOX_STDCALL cloxCodeBlockReaderPeek(const CloxCodeBlockReader_t *const codeBlockReader, const uint32_t offset)
{
    assert(codeBlockReader != NULL);

    CLOX_REGISTER byte_t result;

    if ((codeBlockReader->index + offset) < codeBlockReader->count)
        result = codeBlockReader->array[codeBlockReader->index + offset];
    else
        fail(CLOX_ERROR_MESSAGE_INDEX_OUT_OF_BOUNDS, NULL);

    return result;
}

CLOX_API void CLOX_STDCALL cloxDeleteCodeBlockReader(CloxCodeBlockReader_t *const codeBlockReader, const bool_t freeArray)
{
    assert(codeBlockReader != NULL);

    if (freeArray)
        free(codeBlockReader->array);

    free(codeBlockReader);

    return;
}
