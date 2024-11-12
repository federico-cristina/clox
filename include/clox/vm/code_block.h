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
 * @brief       In this header is defined a data structure to store a block
 *              of code. Providing a dynamic auto-resized array of bytes.
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

CLOX_C_HEADER_END

#endif /* CLOX_VM_CODE_BLOCK_H_ */
