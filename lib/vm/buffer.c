/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/alloc.h"

#include "clox/vm/buffer.h"

#ifndef clox_AdjustBufferCapacity
#   define clox_AdjustBufferCapacity(capacity) alignto(capacity, CLOX_SIZEOF_WORD_PTR)
#endif

CLOX_INLINE CloxBuffer_t *CLOX_STDCALL clox_InitBuffer(CloxBuffer_t *const buffer, size_t capacity)
{
    if (capacity)
    {
        capacity = clox_AdjustBufferCapacity(capacity);

        buffer->array = dim(byte_t, capacity);
        buffer->count = 0;
        buffer->capacity = capacity;
    }
    else
    {
        buffer->array = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }

    return buffer;
}

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxInitBuffer(CloxBuffer_t *const buffer, size_t capacity)
{
    return clox_InitBuffer(buffer, capacity);
}

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxCreateBuffer(size_t capacity)
{
    return clox_InitBuffer(alloc(CloxBuffer_t), capacity);
}

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferResize(CloxBuffer_t *const buffer, size_t newCapacity)
{
    assert(buffer != NULL);

    if (!newCapacity)
    {
        dealloc(buffer->array);

        buffer->count = 0;
        buffer->capacity = 0;
    }
    else
    {
        newCapacity = clox_AdjustBufferCapacity(newCapacity);

        if (newCapacity != buffer->capacity)
        {
            byte_t *newArray;

            if (buffer->array)
            {
                newArray = (byte_t *)realloc(buffer->array, newCapacity);

                if (newArray)
                    buffer->array = newArray;
                else
                    failno("cannot reallocate memory");

                if (buffer->count > newCapacity)
                    buffer->count = newCapacity - 1;

                buffer->capacity = newCapacity;
            }
            else
            {
                clox_InitBuffer(buffer, newCapacity);
            }
        }
    }

    return buffer;
}

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferExpand(CloxBuffer_t *const buffer, uint32_t offset)
{
    assert(buffer != NULL);

    CloxBuffer_t *result;

    if (buffer->capacity < SIZE_MAX)
        result = cloxBufferResize(buffer, buffer->capacity + offset);
    else
        fail("buffer overflow", NULL);

    return result;
}

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferShrink(CloxBuffer_t *const buffer, uint32_t offset)
{
    assert(buffer != NULL);

    CloxBuffer_t *result;

    if (buffer->capacity > 0)
        result = cloxBufferResize(buffer, buffer->capacity - offset);
    else
        fail("buffer underflow", NULL);

    return result;
}

CLOX_INLINE CloxBuffer_t *CLOX_STDCALL clox_BufferGrow(CloxBuffer_t *const buffer)
{
    CLOX_REGISTER uint32_t offset = buffer->capacity;

    if (offset)
        offset >>= 1;
    else
        offset = CLOX_SIZEOF_WORD_PTR;

    return cloxBufferExpand(buffer, offset);
}

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferPush(CloxBuffer_t *const buffer, byte_t value)
{
    assert(buffer != NULL);

    if (buffer->count >= buffer->capacity)
        clox_BufferGrow(buffer);

    buffer->array[buffer->count++] = value;

    return buffer;
}

CLOX_API byte_t CLOX_STDCALL cloxBufferTop(const CloxBuffer_t *const buffer)
{
    assert(buffer != NULL);

    CLOX_REGISTER byte_t result;

    if (buffer->count > 0)
        result = buffer->array[buffer->count - 1];
    else
        fail("buffer underflow", NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxBufferPop(CloxBuffer_t *const buffer)
{
    assert(buffer != NULL);

    CLOX_REGISTER byte_t result;

    if (buffer->count > 0)
    {
        result = buffer->array[--buffer->count];

#ifdef CLOX_BUFFER_AUTO_SHRINK
        if (buffer->count < (buffer->capacity - (2 * CLOX_SIZEOF_WORD_PTR)))
            cloxBufferShrink(buffer, CLOX_SIZEOF_WORD_PTR);
#endif
    }
    else
    {
        fail("buffer underflow", NULL);
    }

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxBufferGet(CloxBuffer_t *const buffer, size_t index)
{
    assert(buffer != NULL);

    CLOX_REGISTER byte_t result;

    if (index < buffer->count)
        result = buffer->array[index];
    else
        fail("index out of bounds", NULL);

    return result;
}

CLOX_API void CLOX_STDCALL cloxDeleteBuffer(CloxBuffer_t *const buffer)
{
    assert(buffer != NULL);

    free(buffer->array);
    free(buffer);

    return;
}

CLOX_API CloxBufferReader_t *CLOX_STDCALL cloxInitBufferReader(CloxBufferReader_t *const bufferReader, const CloxBuffer_t *const buffer)
{
    assert(bufferReader != NULL);

    bufferReader->array = buffer->array;
    bufferReader->count = buffer->count;
    bufferReader->index = 0;

    return bufferReader;
}

CLOX_API CloxBufferReader_t *CLOX_STDCALL cloxCreateBufferReader(const CloxBuffer_t *const buffer)
{
    return cloxInitBufferReader(alloc(CloxBufferReader_t), buffer);
}

CLOX_API byte_t CLOX_STDCALL cloxBufferReaderPeek(CloxBufferReader_t *const bufferReader)
{
    assert(bufferReader != NULL);

    CLOX_REGISTER byte_t result;

    if (bufferReader->index < bufferReader->count)
        result = bufferReader->array[bufferReader->index];
    else
        fail("index out of bounds", NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxBufferReaderNext(CloxBufferReader_t *const bufferReader)
{
    assert(bufferReader != NULL);

    CLOX_REGISTER byte_t result;

    if (bufferReader->index < bufferReader->count)
        result = bufferReader->array[bufferReader->index++];
    else
        fail("buffer overrun", NULL);

    return result;
}

CLOX_API byte_t CLOX_STDCALL cloxBufferReaderBack(CloxBufferReader_t *const bufferReader)
{
    assert(bufferReader != NULL);

    CLOX_REGISTER byte_t result;

    if (bufferReader->index > 0)
        result = bufferReader->array[--bufferReader->index];
    else
        fail("buffer underrun", NULL);

    return result;
}

CLOX_API void CLOX_STDCALL cloxDeleteBufferReader(CloxBufferReader_t *const bufferReader)
{
    assert(bufferReader != NULL);

    free(bufferReader->array);
    free(bufferReader);

    return;
}
