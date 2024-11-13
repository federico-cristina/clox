#pragma once

/**
 * @file        code_block.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 * 
 * @brief       In this header are defined data structures and functions to
 *              deal, dynamically and statically, with blocks of bytecode.
 */

#ifndef CLOX_VM_CODE_BLOCK_H_
#define CLOX_VM_CODE_BLOCK_H_

#include "clox/base/api.h"
#include "clox/base/bits.h"
#include "clox/base/byte.h"

#ifndef cloxAlignToWordPtr
/**
 * @brief       This macro aligns a specified size to the size of a system
 *              word pointer.
 */
#   define cloxAlignToWordPtr(size) alignto(size, CLOX_SIZEOF_WORD_PTR)
#endif

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    CODE_BLOCK Code Block
 * @{
 */

#pragma region Code Block

/**
 * @brief       This data structure provides a dynamic container to store a
 *              block of bytecode (so a dynamic sequence of bytes).
 */
typedef struct _CloxCodeBlock
{
    /**
     * @brief   A pointer to the dynamic array of bytecodes.
     */
    byte_t *array;
    /**
     * @brief   The number of bytes alredy used in the array.
     */
    size_t  count;
    /**
     * @brief   The number of total bytes usable from the array. Capacity
     *          is automatically and dynamically increased.
     */
    size_t  capacity;
} CloxCodeBlock_t;

/**
 * @brief       This function initializes a CloxCodeBlock_t data structure
 *              allocating an initial data array with as bytes as specified
 *              by capacity parameter.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance to initialize.
 * @param       capacity The initial capacity of the block.
 * @return      On success this function returns a pointer to the initialized
 *              code block (so the value of codeBlock parameter).
 */
CLOX_API CloxCodeBlock_t *CLOX_STDCALL cloxInitCodeBlock(CloxCodeBlock_t *const codeBlock, size_t capacity);
/**
 * @brief       This function releases memory blocks used by the specific
 *              instance of the CloxCodeBlock_t function and resets its fields
 *              to default values. The free CloxCodeBlock_t can be reused since
 *              this function doesn't delete it.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance to free.
 * @return      On success this function returns a pointer to the initialized
 *              code block (so the value of codeBlock parameter).
 */
CLOX_API CloxCodeBlock_t *CLOX_STDCALL cloxFreeCodeBlock(CloxCodeBlock_t *const codeBlock);

/**
 * @brief       This function allocates a new CloxCodeBlock_t instance on the
 *              heap and initializes it with the specified initial capacity.
 * 
 * @param       capacity The initial capacity of the block.
 * @return      On success this function returns a pointer to the just allocated
 *              CloxCodeBlock_t instance.
 */
CLOX_API CloxCodeBlock_t *CLOX_STDCALL cloxCreateCodeBlock(const size_t capacity);

/**
 * @brief       This function resizes the specified CloxCodeBlock_t instance.
 * 
 * @param       codeBlock A pointe to the CloxCodeBlock_t instance to resize.
 * @param       newCapacity The new capacity of the block.
 */
CLOX_API void CLOX_STDCALL cloxCodeBlockResize(CloxCodeBlock_t *const codeBlock, size_t newCapacity);
/**
 * @brief       This function expands the specified CloxCodeBlock_t instance
 *              with the specified POSITIVE offset.
 * 
 * @param       codeBlock A pointe to the CloxCodeBlock_t instance to expand.
 * @param       offset The number of bytes to add to the current capacity.
 */
CLOX_API void CLOX_STDCALL cloxCodeBlockExpand(CloxCodeBlock_t *const codeBlock, const uint32_t offset);
/**
 * @brief       This function shrinks the specified CloxCodeBlock_t instance
 *              with the specified NEGATIVE offset.
 * 
 * @param       codeBlock A pointe to the CloxCodeBlock_t instance to shrink.
 * @param       offset The number of bytes to subtract to the current capacity.
 */
CLOX_API void CLOX_STDCALL cloxCodeBlockShrink(CloxCodeBlock_t *const codeBlock, const uint32_t offset);
/**
 * @brief       This function appends a single bytes at the end of the block
 *              growing the buffer if necessary.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance to which
 *              append the byte.
 * @param       value The byte to append to the block.
 * @return      On success this function returns the value of the appended byte
 *              (so the value of value parameter).
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockPush(CloxCodeBlock_t *const codeBlock, const byte_t value);
/**
 * @brief       This function peeks a byte under as bytes as specified by the
 *              offset parameter.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance from which
 *              peek.
 * @param       offset The distance to the requested byte.
 * @return      On success this function returns the value of the specified byte,
 *              but on failure raises a fatal error.
 * 
 * @exception   Index out of range
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockPeek(const CloxCodeBlock_t *const codeBlock, const uint32_t offset);

/**
 * @brief       This function pops the last inserted byte in the block, getting
 *              its value and marking it as overwritable.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance from which
 *              pop.
 * @return      On success this function returns the value of the popped byte, but
 *              on failure a fatal error will be raised.
 * 
 * @exception   Buffer underrun
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockPop(CloxCodeBlock_t *const codeBlock);
/**
 * @brief       This function gets the last inserted byte into the block, getting
 *              its value.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance from which
 *              read the last byte.
 * @return      On success this function returns the value of the last appended
 *              byte, but on failure a fatal error will be raised.
 * 
 * @exception   Buffer underrun
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockTop(const CloxCodeBlock_t *const codeBlock);
/**
 * @brief       This function gets the correspondig byte to the specified index.
 * 
 * @param       codeBlock A pointe to the CloxCodeBlock_t instance from which take
 *              the value.
 * @param       index A number representig the index of the requested byte.
 * @return      On success this function returns the value of the byte at the index,
 *              but on failure a fatal error will be raised.
 * 
 * @exception   Index out of range
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockGet(const CloxCodeBlock_t *const codeBlock, const size_t index);

/**
 * @brief       This function writes a sequence of bytes from a buffer directly to
 *              the specified block, resizing it if necessary.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance in which write.
 * @param       buffer A buffer of bytes, containing the values to write into the
 *              code block.
 * @param       count The number of bytes stored into the buffer parameter.
 * @return      On success this function returns a pointer to the written bytes (so
 *              the value of buffer parameter).
 */
CLOX_API const byte_t *CLOX_STDCALL cloxCodeBlockWrite(CloxCodeBlock_t *const codeBlock, const byte_t *const buffer, const size_t count);

/**
 * @brief       This function deletes a CloxCodeBlock_t heap-allocated instance,
 *              releasing used resources and itself. Use it after cloxCreateCodeBlock
 *              function to clean memory.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance to delete.
 */
CLOX_API void CLOX_STDCALL cloxDeleteCodeBlock(CloxCodeBlock_t *const codeBlock);

#pragma endregion

/**
 * @}
 * 
 * @defgroup    CODE_BLOCK_READER Code Block Reader
 * @{
 */

#pragma region Code Block Reader

/**
 * @brief       This data structure provides a CloxCodeBlock_t byte reader
 *              to run CloxCodeBlock_t data checking bounds.
 * 
 * @note        This data structure is designed to work with a CloxCodeBlock_t
 *              instance, but it can be used also with a common array of
 *              bytes with the cloxInitCodeBlockReaderFromBuffer function.
 */
typedef struct _CloxCodeBlockReader
{
    /**
     * @brief   A pointer to the internal byte array to read, ususally imported
     *          from a CloxCodeBlock_t.
     * 
     * @note    On delete and free operations resources used by this array are
     *          released only if specified, since (as said before) this pointer
     *          is usually shared with a CloxCodeBlock_t instance.
     */
    byte_t *array;
    /**
     * @brief   The maximum number of bytes that can be ridden (usually is equal
     *          to the size of the array).
     */
    size_t  count;
    /**
     * @brief   The number of alredy ridden items from the array (the current
     *          position).
     */
    size_t  index;
} CloxCodeBlockReader_t;

/**
 * @brief       This function initializes a CloxCodeBlockReader_t instance using
 *              a CloxCodeBlock_t instance's data.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              to initialize.
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance from which
 *              inherit data.
 * @return      On success this function returns a pointer to the initialized
 *              CloxCodeBlockReader_t instance (so the value of codeBlockReader
 *              parameter).
 */
CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxInitCodeBlockReader(CloxCodeBlockReader_t *const codeBlockReader, const CloxCodeBlock_t *const codeBlock);
/**
 * @brief       This function initializes a CloxCodeBlockReader_t instace using
 *              a pointer to a raw sequence of bytes into the memory.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              to initialize.
 * @param       buffer A pointer to the sequence of bytes to read.
 * @param       count The number of bytes into the above specified sequence.
 * @return      On success this function returns a pointer to the initialized
 *              CloxCodeBlockReader_t instance (so the value of codeBlockReader
 *              parameter).
 */
CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxInitCodeBlockReaderFromBuffer(CloxCodeBlockReader_t *const codeBlockReader, const byte_t *const buffer, const size_t count);
/**
 * @brief       This function releases resources used by a CloxCodeBlockReader_t
 *              instance, except for the internal byte buffer which eliminination
 *              must be specified with the freeArray parameter.
 * 
 * @note        Use this function combined to cloxInitCodeBlockReader or with
 *              cloxInitCodeBlockReaderFromBuffer functions.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              to free.
 * @param       freeArray When set to TRUE, this flag, specifies that the internal
 *              byte array can be released and deleted.
 * @return      On success this function returns a pointer to the freed empty
 *              CloxCodeBlockReader_t instance (so the value of codeBlockReader
 *              parameter).
 */
CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxFreeCodeBlockReader(CloxCodeBlockReader_t *const codeBlockReader, const bool_t freeArray);

/**
 * @brief       This function allocates on the haep a new CloxCodeBlockReader_t
 *              instance and initializes it using the specified CloxCodeBlock_t
 *              instance's data.
 * 
 * @param       codeBlock A pointer to the CloxCodeBlock_t instance from which
 *              inherit data.
 * @return      On success this function returns a pointer to the just allocated
 *              CloxCodeBlockReader_t instance.
 */
CLOX_API CloxCodeBlockReader_t *CLOX_STDCALL cloxCreateCodeBlockReader(const CloxCodeBlock_t *const codeBlock);

/**
 * @brief       This function gets the next byte and increases the positional
 *              index.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              from which read the next byte.
 * @return      On success this function returns the next byte in the sequence,
 *              but on failure it raises a fatal error.
 * 
 * @exception   Buffer overrun
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockReaderGet(CloxCodeBlockReader_t *const codeBlockReader);
/**
 * @brief       This function gets the next byte and without increasing the
 *              positional index.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              from which peeks the next byte.
 * @return      On success this function returns the top byte in the sequence,
 *              but on failure it raises a fatal error.
 * 
 * @exception   Buffer overrun
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockReaderTop(const CloxCodeBlockReader_t *const codeBlockReader);

/**
 * @brief       This function read a sequence of bytes from the specified code
 *              block reader writing it into the specified buffer.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              from which read.
 * @param       outBuffer A pointer to the buffer into which write the read bytes.
 * @param       count The maximun number of bytes to read.
 * @return      On success this function returns the number of bytes ridden.
 */
CLOX_API size_t CLOX_STDCALL cloxCodeBlockReaderRead(CloxCodeBlockReader_t *const codeBlockReader, byte_t *const outBuffer, const size_t count);
/**
 * @brief       This function peeks a byte with a specific offset.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              from which peek.
 * @param       offset The offset of the byte to peek.
 * @return      On success this function returns the byte at the specified offset.
 */
CLOX_API byte_t CLOX_STDCALL cloxCodeBlockReaderPeek(const CloxCodeBlockReader_t *const codeBlockReader, const uint32_t offset);

/**
 * @brief       This function checks if a specified CloxCodeBlockReader_t
 *              instace has reached the end of the internal buffer.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instace
 *              to check.
 * @return      When the CloxCodeBlockReader_t is at the end this function
 *              returns TRUE, FALSE in other cases.
 */
CLOX_INLINE bool_t CLOX_STDCALL cloxCodeBlockReaderIsAtEnd(const CloxCodeBlockReader_t *const codeBlockReader)
{
    return assert(codeBlockReader != NULL), codeBlockReader->index < codeBlockReader->count;
}

/**
 * @brief       This function deletes a CloxCodeBlockReader_t instance allocated
 *              into the heap, except for the internal byte buffer which
 *              eliminination must be specified with the freeArray parameter.
 * 
 * @note        Use this function combined only with cloxCreateCodeBlockReader
 *              function.
 * 
 * @param       codeBlockReader A pointer to the CloxCodeBlockReader_t instance
 *              to delete.
 * @param       freeArray When set to TRUE, this flag, specifies that the internal
 *              byte array can be released and deleted.
 */
CLOX_API void CLOX_STDCALL cloxDeleteCodeBlockReader(CloxCodeBlockReader_t *const codeBlockReader, const bool_t freeArray);

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_VM_CODE_BLOCK_H_ */
