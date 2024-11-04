#pragma once

/**
 * @file        source_stream.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox scripting language project,
 *              under the Apache License v2.0. See LICENSE for license
 *              informations.
 *
 * @brief       In this header are defined structures and functions to
 *              create, delete and manipulate source streams.
 */

#ifndef CLOX_SOURCE_SOURCE_STREAM_H_
#define CLOX_SOURCE_SOURCE_STREAM_H_

#include "clox/source/source_buffer.h"
#include "clox/source/source_location.h"

#ifndef CLOX_DEFAULT_ENCODING
/**
 * @brief       This constant macro represents the default encoding used by
 *              new source streams.
 */
#   define CLOX_DEFAULT_ENCODING CLOX_SOURCE_ENCODING_UTF_8
#endif

CLOX_C_HEADER_BEGIN

/**
 * @brief       Source stream data structure.
 */
typedef struct _CloxSourceStream
{
    /**
     * @brief   The textual path to the source buffer, or simply the name
     *          of the source stream.
     */
    char                *path;
    /**
     * @brief   A pointer to an open file stream to use to refill the source
     *          buffer when the cursor reaches the end.
     */
    FILE                *stream;
    /**
     * @brief   When it's set to TRUE the source file is treated as an
     *          interactive stream and is read line by line.
     */
    bool_t               isStdin;
    /**
     * @brief   When it's set to TRUE means that the source stream has alredy
     *          been initialized.
     */
    bool_t               isInitialized;
    /**
     * @brief   When it's set to TRUE means that the source file is alredy
     *          open.
     */
    bool_t               isOpen;
    /**
     * @brief   When it's set to TRUE the path string is released on stream
     *          deletion.
     */
    bool_t               cleanup;
    /**
     * @brief   The encoding of the source stream (the default encoding is
     *          specified by CLOX_DEFAULT_ENCODING constant).
     */
    CloxSourceEncoding_t encoding;
    /**
     * @brief   A pointer to the source buffer data structure that stores
     *          the content of the stream.
     */
    CloxSourceBuffer_t  *buffer;
    /**
     * @brief   The location in the source file stream.
     */
    CloxSourceLocation_t streamLocation;
    /**
     * @brief   The current lexeme beginning location.
     */
    CloxSourceLocation_t beginLocation;
    /**
     * @brief   The current lexeme ending location.
     */
    CloxSourceLocation_t forwardLocation;
} CloxSourceStream_t;

/**
 * @brief       Creates a new source stream using as a source to fill the buffer
 *              a NUL-terminated string of characters.
 * 
 * @param       text The string of characters o wrap into the new source stream.
 * @param       encoding The encoding of the stream.
 * @return      A pointer to the new source stream.
 */
CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxCreateSourceStreamFromText(const char *const text, CloxSourceEncoding_t encoding);
/**
 * @brief       Creates a new source stream filling the buffer only once loading
 *              the full content of the specified file.
 * 
 * @param       path The path to the file to load.
 * @param       cleanupPath This flag specifies if the path must be deleted.
 * @param       encoding The encoding of the stream.
 * @return      A pointer to the new source stream.
 */
CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxCreateSourceStreamFromFile(const char *const path, bool_t cleanupPath, CloxSourceEncoding_t encoding);

/**
 * @brief       Creates a new source stream filling the source buffer only once
 *              loading the full content of specified stream.
 * 
 * @param       stream The file stream to load.
 * @param       encoding The encoding of the stream.
 * @return      A pointer to the new source stream.
 */
CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxCreateSourceStreamFromStream(FILE *const stream, CloxSourceEncoding_t encoding);

/**
 * @brief       Opens a new source stream from a file specified by tha path
 *              parameter.
 * 
 * @param       path The path to the file to open.
 * @param       cleanupPath This flag specifies if the path must be deleted.
 * @param       encoding The encoding of the stream.
 * @return      A pointer to the new source stream.
 */
CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxOpenSourceStream(const char *const path, bool_t cleanupPath, CloxSourceEncoding_t encoding);
/**
 * @brief       Opens the source stream that uses stdin as source file. This
 *              function is a special case of the cloxOpenSourceStream function.
 * 
 * @return       A pointer to the new source stream.
 */
CLOX_API CloxSourceStream_t *CLOX_STDCALL cloxOpenStandardSourceStream(void);

/**
 * @brief       Peeks the next character of the stream and returns its value.
 * 
 * @param       sourceStream A pointer to the source stream from which peek the
 *              character.
 * @return      The value of the read character. 
 */
CLOX_API int32_t CLOX_STDCALL cloxSourceStreamPeek(CloxSourceStream_t *const sourceStream);
/**
 * @brief       Reads the next character of the stream and returns its value.
 * 
 * @param       sourceStream A pointer to the source stream from which read the
 *              character.
 * @return      The value of the read character.
 */
CLOX_API int32_t CLOX_STDCALL cloxSourceStreamRead(CloxSourceStream_t *const sourceStream);

/**
 * @brief       Peeks the character next to the specified offset from the stream.
 * 
 * @param       sourceStream A pointer to the source stream from which peek the
 *              character.
 * @param       offset The offset of the character to read.
 * @return      The value of the read character.
 */
CLOX_API int32_t CLOX_STDCALL cloxSourceStreamPeekOffset(CloxSourceStream_t *const sourceStream, uint32_t offset);
/**
 * @brief       Reads the character next to the specified offset from the stream.
 * 
 * @param       sourceStream A pointer to the source stream from which read the
 *              character.
 * @param       offset The offset of the character to read.
 * @return      The value of the read character. 
 */
CLOX_API int32_t CLOX_STDCALL cloxSourceStreamReadOffset(CloxSourceStream_t *const sourceStream, uint32_t offset);

/**
 * @brief       Closes an open source stream.
 * 
 * @param       sourceStream A pointer to the source stream to close.
 * @return      TRUE when the stream is closed succefully, FALSE in the other cases.
 */
CLOX_API bool_t CLOX_STDCALL cloxCloseSourceStream(CloxSourceStream_t *const sourceStream);
/**
 * @brief       Clear the content of the source buffer of the specified source stream.   
 * 
 * @param       sourceStream A pointer to the source stream to clear.
 * @return      TRUE when the stream is cleared succefully, FALSE in the other cases.
 */
CLOX_API bool_t CLOX_STDCALL cloxClearSourceStream(CloxSourceStream_t *const sourceStream);

/**
 * @brief       Deletes a source stream, closing the file stream, clearing the source
 *              buffer and releasing the resources associated to it.
 * 
 * @param       sourceStream A pointer to the source stream to delete.
 */
CLOX_API void CLOX_STDCALL cloxDeleteSourceStream(CloxSourceStream_t *const sourceStream);

CLOX_C_HEADER_END

#endif /* CLOX_SOURCE_SOURCE_STREAM_H_ */
