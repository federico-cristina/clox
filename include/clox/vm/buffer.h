#pragma once

/**
 * @file        buffer.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 */

#ifndef CLOX_VM_BUFFER_H_
#define CLOX_VM_BUFFER_H_

#include "clox/base/api.h"
#include "clox/base/bits.h"
#include "clox/base/byte.h"

CLOX_C_HEADER_BEGIN

typedef struct _CloxBuffer
{
    byte_t *array;
    size_t  count;
    size_t  capacity;
} CloxBuffer_t;

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxInitBuffer(CloxBuffer_t *const buffer, size_t capacity);
CLOX_API CloxBuffer_t *CLOX_STDCALL cloxCreateBuffer(size_t capacity);

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferResize(CloxBuffer_t *const buffer, size_t newCapacity);
CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferExpand(CloxBuffer_t *const buffer, uint32_t offset);
CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferShrink(CloxBuffer_t *const buffer, uint32_t offset);

CLOX_API CloxBuffer_t *CLOX_STDCALL cloxBufferPush(CloxBuffer_t *const buffer, byte_t value);

CLOX_API byte_t CLOX_STDCALL cloxBufferTop(const CloxBuffer_t *const buffer);
CLOX_API byte_t CLOX_STDCALL cloxBufferPop(CloxBuffer_t *const buffer);
CLOX_API byte_t CLOX_STDCALL cloxBufferGet(CloxBuffer_t *const buffer, size_t index);

CLOX_API void CLOX_STDCALL cloxDeleteBuffer(CloxBuffer_t *const buffer);

typedef struct _CloxBufferReader
{
    byte_t *array;
    size_t  count;
    size_t  index;
} CloxBufferReader_t;

CLOX_API CloxBufferReader_t *CLOX_STDCALL cloxInitBufferReader(CloxBufferReader_t *const bufferReader, const CloxBuffer_t *const buffer);
CLOX_API CloxBufferReader_t *CLOX_STDCALL cloxCreateBufferReader(const CloxBuffer_t *const buffer);

CLOX_API byte_t CLOX_STDCALL cloxBufferReaderPeek(CloxBufferReader_t *const bufferReader);
CLOX_API byte_t CLOX_STDCALL cloxBufferReaderNext(CloxBufferReader_t *const bufferReader);
CLOX_API byte_t CLOX_STDCALL cloxBufferReaderBack(CloxBufferReader_t *const bufferReader);

CLOX_INLINE bool_t CLOX_STDCALL cloxBufferReaderAtEnd(const CloxBufferReader_t *const bufferReader)
{
    return bufferReader->index >= bufferReader->count;
}

CLOX_API void CLOX_STDCALL cloxDeleteBufferReader(CloxBufferReader_t *const bufferReader);

CLOX_C_HEADER_END

#endif /* CLOX_VM_BUFFER_H_ */
