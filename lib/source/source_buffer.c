/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/string.h"
#include "clox/base/utf8.h"

#include "clox/source/source_buffer.h"

CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBuffer(size_t size, byte_t *const content, size_t count)
{
    CloxSourceBuffer_t *sourceBuffer = alloc(CloxSourceBuffer_t);

    if (!content || !count)
        sourceBuffer->data = dim(byte_t, size);
    else
        sourceBuffer->data = bufcpy(dim(byte_t, size), content, count);

    sourceBuffer->size = size;

    return sourceBuffer;
}

CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromText(const char *const text)
{
    size_t length;

    if (!text)
        length = 0;
    else
        length = strlen(text);

    return cloxCreateSourceBuffer(length + 1, (byte_t *)text, length);
}

CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromFile(const char *const path)
{
    assert(path != NULL);

    FILE *stream = fopen(path, CLOX_LOADMOD);
    CloxSourceBuffer_t *sourceBuffer;

    if (!stream)
        sourceBuffer = NULL;
    else
        sourceBuffer = cloxCreateSourceBufferFromStream(stream), fclose(stream);

    return sourceBuffer;
}

CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromStream(FILE *const stream)
{
    assert(stream != NULL);

    size_t size = fgetsiz(stream);
    size_t fpos = 0;

    CloxSourceBuffer_t *sourceBuffer = cloxCreateSourceBuffer(size, NULL, 0);

    byte_t *p = sourceBuffer->data;

    do
        fpos += fread(p + fpos, sizeof(byte_t), min(CLOX_PAGESIZ, (size - fpos)), stream);
    while (fpos < size);

    p[fpos] = NUL;

    return sourceBuffer;
}

CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromStdin(void)
{
    CloxSourceBuffer_t *sourceBuffer = cloxCreateSourceBuffer(CLOX_PAGESIZ, NULL, 0);

    char *buffer = fgets((char *)sourceBuffer->data, CLOX_PAGESIZ, stdin), *newBuffer;
    size_t length = strlen(buffer);

    if (length < CLOX_PAGESIZ)
    {
        newBuffer = (char *)realloc((void *)buffer, length + 1);

        if (newBuffer)
        {
            sourceBuffer->data = (byte_t *)newBuffer;
            sourceBuffer->size = length + 1;
        }
    }

    return sourceBuffer;
}

CLOX_API int32_t CLOX_STDCALL cloxSourceBufferGetChar(CloxSourceBuffer_t *const sourceBuffer, CloxSourceEncoding_t encoding, uint64_t position, ssize_t *const outOffset)
{
    int32_t result;
    ssize_t offset;

    if (position < sourceBuffer->size)
    {
        switch (encoding)
        {
        case CLOX_SOURCE_ENCODING_ASCII:
            result = sourceBuffer->data[position];
            offset = 1;
            break;

        case CLOX_SOURCE_ENCODING_UTF_8:
            offset = utf8_iterate((const uint8_t *)(sourceBuffer->data + position), (ssize_t)(sourceBuffer->size - position), &result);
            break;

        default:
            return unreach(), EOF;
        }
    }
    else
    {
        result = EOF;
    }

    if (outOffset && *outOffset)
        *outOffset = offset;

    return result;
}

CLOX_API int CLOX_STDCALL cloxDumpSourceBuffer(const CloxSourceBuffer_t *const sourceBuffer, FILE *const stream)
{
    return fputs((const char *)sourceBuffer->data, !stream ? stderr : stream);
}

CLOX_API bool_t CLOX_STDCALL cloxClearSourceBuffer(CloxSourceBuffer_t *const sourceBuffer)
{
    return (bool_t)(!!bufclr(sourceBuffer->data, sourceBuffer->size));
}

CLOX_API void CLOX_STDCALL cloxDeleteSourceBuffer(CloxSourceBuffer_t *const sourceBuffer)
{
    free(sourceBuffer->data);
    free(sourceBuffer);

    return;
}
