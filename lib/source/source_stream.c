/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/alloc.h"

#include "clox/source/source_stream.h"

CLOX_INLINE CloxSourceStream_t *CLOX_STDCALL clox_InitializeSourceStream(CloxSourceStream_t *const sourceStream)
{
    sourceStream->path = NULL;
    sourceStream->stream = NULL;

    sourceStream->isStdin = FALSE;
    sourceStream->isInitialized = FALSE;
    sourceStream->isOpen = FALSE;
    sourceStream->cleanup = FALSE;

    sourceStream->buffer = NULL;

    cloxResetSourceLocation(&sourceStream->streamLocation);
    cloxResetSourceLocation(&sourceStream->beginLocation);
    cloxResetSourceLocation(&sourceStream->forwardLocation);

    return sourceStream;
}

CLOX_INLINE CloxSourceStream_t *CLOX_STDCALL clox_CreateSourceStream(const char *const path, FILE *const stream, bool_t isStdin, bool_t isInitialized, bool_t isOpen, bool_t cleanup, CloxSourceEncoding_t encoding, CloxSourceBuffer_t *const sourceBuffer)
{
    CloxSourceStream_t *sourceStream = alloc(CloxSourceStream_t);

    sourceStream->path = (char *)path;
    sourceStream->stream = stream;

    sourceStream->isStdin = isStdin;
    sourceStream->isInitialized = isInitialized;
    sourceStream->isOpen = isOpen;
    sourceStream->cleanup = cleanup;

    sourceStream->encoding = encoding;
    sourceStream->buffer = sourceBuffer;

    cloxResetSourceLocation(&sourceStream->streamLocation);
    cloxResetSourceLocation(&sourceStream->beginLocation);
    cloxResetSourceLocation(&sourceStream->forwardLocation);

    return sourceStream;
}

CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxCreateSourceStreamFromText(const char *const text, CloxSourceEncoding_t encoding)
{
    return clox_CreateSourceStream(NULL, NULL, FALSE, FALSE, FALSE, FALSE, encoding, cloxCreateSourceBufferFromText(text));
}

CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxCreateSourceStreamFromFile(const char *const path, bool_t cleanupPath, CloxSourceEncoding_t encoding)
{
    assert(path != NULL);

    CloxSourceBuffer_t *sourceBuffer = cloxCreateSourceBufferFromFile(path);

    if (!sourceBuffer)
        return NULL;
    else
        return clox_CreateSourceStream(path, NULL, FALSE, FALSE, FALSE, cleanupPath, encoding, sourceBuffer);
}

CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxCreateSourceStreamFromStream(FILE *const stream, CloxSourceEncoding_t encoding)
{
    assert(stream != NULL);

    CloxSourceBuffer_t *sourceBuffer = cloxCreateSourceBufferFromStream(stream);

    if (!sourceBuffer)
        return NULL;
    else
        return clox_CreateSourceStream(NULL, NULL, FALSE, FALSE, FALSE, FALSE, encoding, sourceBuffer);
}

CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxOpenSourceStream(const char *const path, bool_t cleanupPath, CloxSourceEncoding_t encoding)
{
    assert(path != NULL);

    FILE *stream = fopen(path, CLOX_LOADMOD);

    if (!stream)
        return NULL;

    return clox_CreateSourceStream(path, stream, FALSE, FALSE, TRUE, cleanupPath, encoding, cloxCreateSourceBuffer(CLOX_PAGESIZ, NULL, 0));
}

CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxOpenStandardSourceStream(void)
{
    return clox_CreateSourceStream("<stdin>", stdin, TRUE, FALSE, TRUE, FALSE, CLOX_DEFAULT_ENCODING, cloxCreateSourceBuffer(CLOX_PAGESIZ, NULL, 0));
}

CLOX_INLINE bool_t CLOX_STDCALL clox_SourceStreamNeedsARefill(CloxSourceStream_t *const sourceStream, uint32_t offset)
{
    return !sourceStream->isInitialized || ((sourceStream->forwardLocation.ch + offset) >= sourceStream->buffer->size);
}

CLOX_INLINE bool_t CLOX_STDCALL clox_SourceStreamRefill(CloxSourceStream_t *const sourceStream)
{
    if (!sourceStream->isOpen || !sourceStream->stream)
        return FALSE;

    FILE *stream = sourceStream->stream;
    byte_t *data = sourceStream->buffer->data;
    size_t begin = sourceStream->beginLocation.ch, count = sourceStream->buffer->size - begin;

    if (feof(stream))
        return FALSE;

    if (begin)
        bufmov(data, (data + begin), count);

    if (!sourceStream->isStdin)
        fread((void *)(data + begin), sizeof(byte_t), count, stream);
    else
        fgets((char *)(data + begin), count, stream);

    if (!sourceStream->isInitialized)
    {
        sourceStream->isInitialized = TRUE;
    }
    else
    {
        CloxSourceLocation_t *beginLoc = &sourceStream->beginLocation, *forwardLoc = &sourceStream->forwardLocation;

        if (begin)
            cloxSetSourceLocation(forwardLoc, forwardLoc->ch - beginLoc->ch, forwardLoc->co - beginLoc->co, forwardLoc->ln - beginLoc->ln);
        else
            cloxResetSourceLocation(forwardLoc);

        cloxResetSourceLocation(beginLoc);
    }

    return TRUE;
}

CLOX_API int32_t CLOX_STDCALL cloxSourceStreamPeek(CloxSourceStream_t *const sourceStream)
{
    if (clox_SourceStreamNeedsARefill(sourceStream, 0) && !clox_SourceStreamRefill(sourceStream))
        return EOF;
    else
        return cloxSourceBufferGetChar(sourceStream->buffer, sourceStream->encoding, sourceStream->forwardLocation.ch, NULL);
}

CLOX_INLINE int32_t CLOX_STDCALL clox_SourceStreamRead(CloxSourceStream_t *const sourceStream, ssize_t *const outOffset)
{
    if (clox_SourceStreamNeedsARefill(sourceStream, 0) && !clox_SourceStreamRefill(sourceStream))
        return EOF;

    int32_t result;
    ssize_t offset;

    result = cloxSourceBufferGetChar(sourceStream->buffer, sourceStream->encoding, sourceStream->forwardLocation.ch, &offset);

    switch (result)
    {
    case EOF:
    case NUL:
        break;

    case EOL:
        sourceStream->streamLocation.co = 0;
        sourceStream->streamLocation.ln++;
        sourceStream->forwardLocation.co = 0;
        sourceStream->forwardLocation.ln++;
        break;

    default:
        sourceStream->streamLocation.co += offset;
        sourceStream->forwardLocation.co += offset;
        break;
    }

    sourceStream->streamLocation.ch += offset;
    sourceStream->forwardLocation.ch += offset;

    if (outOffset && *outOffset)
        *outOffset = offset;

    return result;
}

CLOX_API int32_t CLOX_STDCALL cloxSourceStreamRead(CloxSourceStream_t *const sourceStream)
{
    return clox_SourceStreamRead(sourceStream, NULL);
}

CLOX_API int32_t CLOX_STDCALL cloxSourceStreamPeekOffset(CloxSourceStream_t *const sourceStream, uint32_t offset)
{
    return cloxSourceBufferGetChar(sourceStream->buffer, sourceStream->encoding, sourceStream->forwardLocation.ch + offset, NULL);
}

CLOX_API int32_t CLOX_STDCALL cloxSourceStreamReadOffset(CloxSourceStream_t *const sourceStream, uint32_t offset)
{
    if (clox_SourceStreamNeedsARefill(sourceStream, offset) && !clox_SourceStreamRefill(sourceStream))
        return EOF;

    int32_t result = EOF;
    uint32_t i;

    for (i = 0; i < offset; i++)
        result = cloxSourceStreamRead(sourceStream);

    return result;
}

CLOX_API bool_t CLOX_STDCALL cloxCloseSourceStream(CloxSourceStream_t *const sourceStream)
{
    if (sourceStream->isOpen)
        return sourceStream->isOpen = (bool_t)fclose(sourceStream->stream);
    else
        return FALSE;
}

CLOX_API bool_t CLOX_STDCALL cloxClearSourceStream(CloxSourceStream_t *const sourceStream)
{
    return cloxClearSourceBuffer(sourceStream->buffer);
}

CLOX_API void CLOX_STDCALL cloxDeleteSourceStream(CloxSourceStream_t *const sourceStream)
{
    cloxCloseSourceStream(sourceStream);
    cloxDeleteSourceBuffer(sourceStream->buffer);

    if (sourceStream->cleanup)
        free(sourceStream->path);

    free(sourceStream);

    return;
}
