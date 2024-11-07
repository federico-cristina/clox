/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/alloc.h"
#include "clox/vm/chunk.h"

#ifndef clox_AdjustChunkCapacity
#   define clox_AdjustChunkCapacity(capacity) alignto(capacity, CLOX_SIZEOF_WORD_PTR)
#endif

CLOX_API CloxChunk_t *CLOX_STDCALL cloxInitChunk(CloxChunk_t *const chunk, size_t capacity)
{
    assert(chunk != NULL);

    if (!capacity)
    {
        chunk->array = NULL;
        chunk->count = 0;
        chunk->capacity = 0;
    }
    else
    {
        capacity = clox_AdjustChunkCapacity(capacity);

        chunk->array = dim(byte_t, capacity);
        chunk->count = 0;
        chunk->capacity = capacity;
    }

    return chunk;
}

CLOX_API CloxChunk_t *CLOX_STDCALL cloxCreateChunk(size_t capacity)
{
    return cloxInitChunk(alloc(CloxChunk_t), capacity);
}

CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkResize(CloxChunk_t *const chunk, size_t newCapacity)
{
    assert(chunk != NULL);

    if (!newCapacity)
    {
        dealloc(chunk->array);

        chunk->count = 0;
        chunk->capacity = 0;
    }
    else
    {
        newCapacity = clox_AdjustChunkCapacity(newCapacity);

        if (newCapacity != chunk->capacity)
        {
            if (chunk->array)
            {
                chunk->array = redim(byte_t, chunk->array, newCapacity);
                
                if (chunk->count > newCapacity)
                    chunk->count = newCapacity - 1;

                chunk->capacity = newCapacity;
            }
            else
            {
                cloxInitChunk(chunk, newCapacity);
            }
        }
    }

    return chunk;
}

CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkExpand(CloxChunk_t *const chunk, uint32_t offset)
{
    return cloxChunkResize(chunk, chunk->capacity + offset);
}

CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkShrink(CloxChunk_t *const chunk, uint32_t offset)
{
    return cloxChunkResize(chunk, chunk->capacity - offset);
}

CLOX_INLINE CloxChunk_t *CLOX_STDCALL clox_ChunkGrow(CloxChunk_t *const chunk)
{
    CLOX_REGISTER uint32_t newCapacity = chunk->capacity;

    if (newCapacity)
        newCapacity *= CLOX_CHUNK_GROWING_FACTOR;
    else
        newCapacity = CLOX_SIZEOF_WORD_PTR;

    return cloxChunkResize(chunk, newCapacity);
}

CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkPush(CloxChunk_t *const chunk, byte_t value)
{
    assert(chunk != NULL);

    if (chunk->count >= chunk->capacity)
        clox_ChunkGrow(chunk);

    chunk->array[chunk->count++] = value;

    return chunk;
}

CLOX_API byte_t CLOX_STDCALL cloxChunkTop(const CloxChunk_t *const chunk)
{
    assert(chunk != NULL);

    CLOX_REGISTER byte_t result;

    if (chunk->count > 0)
        result = chunk->array[chunk->count - 1];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_UNDERRUN, NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxChunkPop(CloxChunk_t *const chunk)
{
    assert(chunk != NULL);

    CLOX_REGISTER byte_t result;

    if (chunk->count > 0)
        result = chunk->array[--chunk->count];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_UNDERRUN, NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxChunkGet(CloxChunk_t *const chunk, size_t index)
{
    assert(chunk != NULL);

    CLOX_REGISTER byte_t result;

    if (index < chunk->count)
        result = chunk->array[index];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_INDEX_OUT_OF_BOUNDS, NULL);

    return result;
}

CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkWrite(CloxChunk_t *const chunk, const byte_t *const buffer, size_t count)
{
    assert(chunk != NULL);

    if ((chunk->count + count) >= chunk->capacity)
        cloxChunkExpand(chunk, (chunk->count + count) - chunk->capacity);

    bufcpy(chunk->array, buffer, count);

    return chunk->count += count, chunk;
}

CLOX_API void CLOX_STDCALL cloxDeleteChunk(CloxChunk_t *const chunk)
{
    assert(chunk != NULL);

    free(chunk->array);
    free(chunk);

    return;
}

#undef clox_AdjustChunkCapacity

CLOX_API CloxChunkReader_t *CLOX_STDCALL cloxInitChunkReader(CloxChunkReader_t *const chunkReader, const CloxChunk_t *const chunk)
{
    assert(chunkReader != NULL);

    if (!chunk)
    {
        chunkReader->array = NULL;
        chunkReader->count = 0;
        chunkReader->index = 0;
    }
    else
    {
        chunkReader->array = chunk->array;
        chunkReader->count = chunk->count;
        chunkReader->index = 0;
    }

    return chunkReader;
}

CLOX_API CloxChunkReader_t *CLOX_STDCALL cloxCreateChunkReader(const CloxChunk_t *const chunk)
{
    return cloxInitChunkReader(alloc(CloxChunkReader_t), chunk);
}

CLOX_API byte_t CLOX_STDCALL cloxChunkReaderTop(const CloxChunkReader_t *const chunkReader)
{
    assert(chunkReader != NULL);

    CLOX_REGISTER byte_t result;

    if (chunkReader->index < chunkReader->count)
        result = chunkReader->array[chunkReader->index];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_INDEX_OUT_OF_BOUNDS, NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxChunkReaderGet(CloxChunkReader_t *const chunkReader)
{
    assert(chunkReader != NULL);

    CLOX_REGISTER byte_t result;

    if (chunkReader->index < chunkReader->count)
        result = chunkReader->array[chunkReader->index++];
    else
        fail(CLOX_ERROR_MESSAGE_BUFFER_OVERRUN, NULL);

    return result;
}

CLOX_API size_t CLOX_STDCALL cloxChunkReaderPeek(const CloxChunkReader_t *const chunkReader, byte_t *const outBuffer, size_t count)
{
    assert(chunkReader != NULL);

    CLOX_REGISTER size_t readCount, i, n;

    if (outBuffer)
    {
        i = chunkReader->index;
        n = chunkReader->count;

        CLOX_REGISTER byte_t *array = chunkReader->array;

        for (readCount = 0; (readCount < count) && (i < n); readCount++, i++)
            outBuffer[readCount] = array[i + readCount];
    }
    else
    {
        readCount = 0;
    }

    return readCount;
}

CLOX_API size_t CLOX_STDCALL cloxChunkReaderRead(CloxChunkReader_t *const chunkReader, byte_t *const outBuffer, size_t count)
{
    assert(chunkReader != NULL);

    CLOX_REGISTER size_t readCount, i, n;

    if (outBuffer)
    {
        i = chunkReader->index;
        n = chunkReader->count;

        CLOX_REGISTER byte_t *array = chunkReader->array;

        for (readCount = 0; (readCount < count) && (i < n); readCount++, i++)
            outBuffer[readCount] = array[i + readCount];

        chunkReader->index = i;
    }
    else
    {
        readCount = 0;
    }

    return readCount;
}

CLOX_API void CLOX_STDCALL cloxDeleteChunkReader(CloxChunkReader_t *const chunkReader, bool_t cleanUpArray)
{
    assert(chunkReader != NULL);

    if (cleanUpArray)
        free(chunkReader->array);
    else
        chunkReader->array = NULL;

    free(chunkReader);

    return;
}
