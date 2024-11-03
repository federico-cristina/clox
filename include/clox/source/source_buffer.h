#pragma once

/**
 * @file        source_buffer.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 *
 * @brief       In this header are defined structures, macro and functions
 *              to create, delete and manipulate source buffers.
 */

#ifndef CLOX_SOURCE_SOURCE_BUFFER_H_
#define CLOX_SOURCE_SOURCE_BUFFER_H_

#include "clox/base/api.h"
#include "clox/base/bits.h"
#include "clox/base/byte.h"
#include "clox/base/file.h"

CLOX_C_HEADER_BEGIN

/**
 * @brief       Enumeration of supported encodings.
 */
typedef enum _CloxSourceEncoding
{
    /**
     * @brief   ASCII encoding.
     */
    CLOX_SOURCE_ENCODING_ASCII = 0x10,
    /**
     * @brief   UTF-8 encoding.
     */
    CLOX_SOURCE_ENCODING_UTF_8 = 0x20,
} CloxSourceEncoding_t;

/**
 * @brief       Source buffer data structure.
 */
typedef struct _CloxSourceBuffer
{
    /**
     * @brief   A pointer to the beginning of the buffer's data.
     */
    byte_t *data;
    /**
     * @brief   The maximum number of characters that this source buffer can
     *          contain.
     */
    size_t  size;
} CloxSourceBuffer_t;

/**
 * @brief       Creates a new source buffer of the same number of characters as
 *              size parameter specifies and an initial content specified by the
 *              content parameter.
 * 
 * @param       size The maximum number of bytes that the source buffer will
 *              store.
 * @param       content A buffer of bytes representing the content that will be
 *              copied in the buffer on its creation. It can be NULL.
 * @param       count The number of bytes stored into content parameter.
 * @return      A pointer to the new source buffer.
 */
CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBuffer(size_t size, byte_t *const content, size_t count);

/**
 * @brief       Creates a new source buffer that will contain a specified string
 *              of bytes.
 * 
 * @param       text The string to wrap into the buffer.
 * @return      A pointer to the new source buffer.
 */
CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromText(const char *const text);
/**
 * @brief       Creates a new source buffers with the entire content of the file
 *              specified by the path parameter.
 * 
 * @param       path The path to the file to load and wrap into the buffer.
 * @return      A pointer to the new source buffer.
 */
CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromFile(const char *const path);

/**
 * @brief       Creates a new source buffer loading the content of a file stream,
 *              do not use this function to load a source buffer form stdin.
 * 
 * @param       stream A source file stream.
 * @return      A pointer to the new source buffer.
 */
CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromStream(FILE *const stream);
/**
 * @brief       Creates a new source buffer loading the next line of the stdin.
 * 
 * @return      A pointer to the new source buffer.
 */
CLOX_API CloxSourceBuffer_t *CLOX_STDCALL cloxCreateSourceBufferFromStdin(void);

/**
 * @brief       Gets thecharacter in the position specified by position parameter.
 * 
 * @param       sourceBuffer A pointer to the source buffer from which read.
 * @param       encoding The encoding of the character to read.
 * @param       position The position of the character to read.
 * @param       outOffset A pointer to a variable in which store the character's
 *              width in bytes.
 * @return      The character in the specified position.
 */
CLOX_API int32_t CLOX_STDCALL cloxSourceBufferGetChar(CloxSourceBuffer_t *const sourceBuffer, CloxSourceEncoding_t encoding, uint64_t position, ssize_t *const outOffset);

/**
 * @brief       Dumps the content of a source buffer on a stream. When stream parameter
 *              is NULL the default choice is stderr stream.
 * 
 * @param       sourceBuffer A pointer to the source buffer from which read.
 * @param       stream The stream on which dump the content of the source buffer.
 * @return      The number of written characters.
 */
CLOX_API int CLOX_STDCALL cloxDumpSourceBuffer(const CloxSourceBuffer_t *const sourceBuffer, FILE *const stream);

/**
 * @brief       Clears the content of the source buffer.
 * 
 * @param       sourceBuffer A pointer to the source buffer to clear.
 * @return      TRUE when the buffer is cleared succefully, FALSE in the other cases.
 */
CLOX_API bool_t CLOX_STDCALL cloxClearSourceBuffer(CloxSourceBuffer_t *const sourceBuffer);

/**
 * @brief       Deletes the specified source buffer releasing each used resource.
 * 
 * @param       sourceBuffer A pointer to the source buffer to delete.
 */
CLOX_API void CLOX_STDCALL cloxDeleteSourceBuffer(CloxSourceBuffer_t *const sourceBuffer);

CLOX_C_HEADER_END

#endif /* CLOX_SOURCE_SOURCE_BUFFER_H_ */
