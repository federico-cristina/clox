#pragma once

/**
 * @file        chunk.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 */

#ifndef CLOX_VM_CHUNK_H_
#define CLOX_VM_CHUNK_H_

#include "clox/base/api.h"
#include "clox/base/bits.h"
#include "clox/base/byte.h"

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    BYTECODE_CHUNKS Bytecode Chunks
 * @{
 */

#pragma region Bytecode Chunks

#ifndef CLOX_CHUNK_GROWING_FACTOR
#   define CLOX_CHUNK_GROWING_FACTOR 2
#endif

typedef struct _CloxChunk
{
    byte_t *array;
    size_t  count;
    size_t  capacity;
} CloxChunk_t;

CLOX_API CloxChunk_t *CLOX_STDCALL cloxInitChunk(CloxChunk_t *const chunk, size_t capacity);
CLOX_API CloxChunk_t *CLOX_STDCALL cloxCreateChunk(size_t capacity);

CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkResize(CloxChunk_t *const chunk, size_t newCapacity);
CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkExpand(CloxChunk_t *const chunk, uint32_t offset);
CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkShrink(CloxChunk_t *const chunk, uint32_t offset);

CLOX_API CloxChunk_t *CLOX_STDCALL cloxChunkPush(CloxChunk_t *const chunk, byte_t value);

CLOX_API byte_t CLOX_STDCALL cloxChunkTop(const CloxChunk_t *const chunk);
CLOX_API byte_t CLOX_STDCALL cloxChunkPop(CloxChunk_t *const chunk);
CLOX_API byte_t CLOX_STDCALL cloxChunkGet(CloxChunk_t *const chunk, size_t index);

CLOX_API void CLOX_STDCALL cloxDeleteChunk(CloxChunk_t *const chunk);

#pragma endregion

/**
 * @}
 * 
 * @defgroup    BYTECODE_CHUNK_READERS Bytecode Chunk Readers
 * @{
 */

#pragma region Bytecode Chunk Readers

typedef struct _CloxChunkReader
{
    byte_t *array;
    size_t  count;
    size_t  index;
} CloxChunkReader_t;

CLOX_API CloxChunkReader_t *CLOX_STDCALL cloxInitChunkReader(CloxChunkReader_t *const chunkReader, const CloxChunk_t *const chunk);
CLOX_API CloxChunkReader_t *CLOX_STDCALL cloxCreateChunkReader(const CloxChunk_t *const chunk);

CLOX_API byte_t CLOX_STDCALL cloxChunkReaderTop(const CloxChunkReader_t *const chunkReader);
CLOX_API byte_t CLOX_STDCALL cloxChunkReaderGet(CloxChunkReader_t *const chunkReader);

CLOX_API size_t CLOX_STDCALL cloxChunkReaderPeek(const CloxChunkReader_t *const chunkReader, byte_t *const outBuffer, size_t count);
CLOX_API size_t CLOX_STDCALL cloxChunkReaderRead(CloxChunkReader_t *const chunkReader, byte_t *const outBuffer, size_t count);

CLOX_INLINE bool_t CLOX_STDCALL cloxChunkReaderIsAtEnd(const CloxChunkReader_t *const chunkReader)
{
    return chunkReader->array && (chunkReader->index < chunkReader->count);
}

CLOX_API void CLOX_STDCALL cloxDeleteChunkReader(CloxChunkReader_t *const chunkReader, bool_t cleanUpArray);

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_VM_CHUNK_H_ */
