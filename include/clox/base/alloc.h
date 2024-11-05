#pragma once

/**
 * @file        alloc.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 *
 * @version     1.1
 *
 * @brief       In this header are defined macros, constants and functions
 *              to allocate and deallocate memory on the stack and on the
 *              heap ensuring memory safety.
 */

#ifndef CLOX_BASE_ALLOC_H_
#define CLOX_BASE_ALLOC_H_

#include "clox/base/errno.h"

#if CLOX_PLATFORM_IS_WINDOWS
#   include <malloc.h>
#endif

CLOX_C_HEADER_BEGIN

/**
 * @brief       Checks if a memory block is a null pointer and in that case
 *              aborts program execution showing an error message.
 * 
 * @param       block Memory block to check.
 * @param       errMsg Message that will be printed on the error stream in
 *              case of failure (when the block is null).
 * @return      The non-null block.
 */
CLOX_INLINE void *CLOX_STDCALL _check(void *const block, const char *const errMsg)
{
    if (!block)
        return failno(errMsg);
    else
        return block;
}

#ifndef CLOX_ALLOC_ERROR_MESSAGE
/**
 * @brief       This constant represent the default error message printed on
 *              the error stream when the allocation of memory fails.
 */
#   define CLOX_ALLOC_ERROR_MESSAGE "cannot allocate memory"
#endif

#ifndef cmalloc
/**
 * @brief       Allocates a block of bytes of the same in the dynamic heap,
 *              checking return value.
 * 
 * @param       S The number of bytes to allocate.
 * @return      A pointer to the new block of memory.
 */
#   define cmalloc(S) _check(malloc((S)), CLOX_ALLOC_ERROR_MESSAGE)
#endif

#ifndef alloc
/**
 * @brief       Allocates a block of bytes of the same dimension of the specified
 *              data type.
 * 
 * @param       T The type to allocate.
 * @return      A pointer to the new block of memory.
 */
#   define alloc(T) (T *)cmalloc(sizeof(T))
#endif

#ifndef ccalloc
/**
 * @brief       Allocates a series of N contiguous blocks of memory in the dynamic
 *              heap, checking return value.
 * 
 * @param       N The number of items to allocate.
 * @param       S The size of each item to allocate.
 * @return      A pointer to the new block of memory.
 */
#   define ccalloc(N, S) _check(calloc((N), (S)), CLOX_ALLOC_ERROR_MESSAGE)
#endif

#ifndef dim
/**
 * @brief       Allocates a series of N contiguous blocks of memory of the same
 *              dimension of the specified data type.
 * 
 * @param       T The type to allocate.
 * @param       N The number of items to allocate.
 * @return      A pointer to the new block of memory.
 */
#   define dim(T, N) (T *)ccalloc(N, sizeof(T))
#endif

#ifndef CLOX_RELOC_ERROR_MESSAGE
/**
 * @brief       This constant represent the default error message printed on
 *              the error stream when reallocation of a block fails.
 */
#   define CLOX_RELOC_ERROR_MESSAGE "cannot reallocate memory"
#endif

#ifndef reloc
/**
 * @brief       Reallocates a block of bytes.
 * 
 * @param       T The type to allocate.
 * @param       B A pointer to the block to reallocate.
 * @param       S The new size of the block.
 * @return      A pointer to the new block of memory.
 */
#   define reloc(T, B, S) (T *)_check(realloc((void *)(B), (S)), CLOX_RELOC_ERROR_MESSAGE)
#endif

#ifndef redim
/**
 * @brief       Reallocates an array of block of bytes.
 * 
 * @param       T The type to allocate.
 * @param       B A pointer to the block to reallocate.
 * @param       N The new dimension of the array.
 * @return      A pointer to the new block of memory.
 */
#   define redim(T, B, N) (T *)reloc(B, sizeof(T) * N)
#endif

/**
 * @brief       Frees a block of memory and sets its pointer to NULL.
 * 
 * @param       blockRef A pointer to the pointer to the block of memory
 *              to free.
 */
CLOX_INLINE void CLOX_STDCALL _dealloc(void **blockRef)
{
    if (blockRef && *blockRef)
    {
        free(*blockRef);
        *blockRef = NULL;
    }

    return;
}

#ifndef dealloc
/**
 * @brief       Frees a block of memory and sets its pointer to NULL.
 * 
 * @param       block A pointer to the block of memory to free.
 */
#   define dealloc(block) _dealloc((void **)&block)
#endif

/**
 * @brief       Frees a block of memory and returns a specified value.
 * 
 * @param       toFree A pointer to the pointer to the block of memory
 *              to free.
 * @param       retValue Value to return as result.
 * @return      The value of retValue.
 */
CLOX_INLINE void *CLOX_STDCALL freeret(void **toFree, void *retValue)
{
    return _dealloc(toFree), retValue;
}

#ifndef CLOX_ALLOCA_ERROR_MESSAGE
/**
 * @brief       This constant represents the default error message printed on
 *              the error stream when the allocation of memory on the stack
 *              fails.
 */
#   define CLOX_ALLOCA_ERROR_MESSAGE "cannot allocate memory on stack"
#endif

#ifndef malloca
#   if CLOX_PLATFORM_IS_WINDOWS
/**
 * @brief       Allocates a block of bytes on the stack checking that's not
 *              NULL and failing in that case.
 * 
 * @param       size The number of bytes to allocate.
 * @return      A pointer to the new block of memory.
 */
#       define malloca(size) _check(_malloca(size), CLOX_ALLOCA_ERROR_MESSAGE)
#   else
/**
 * @brief       Allocates a block of bytes on the stack checking that's not
 *              NULL and failing in that case.
 * 
 * @param       size The number of bytes to allocate.
 * @return      A pointer to the new block of memory.
 */
#       define malloca(size) _check(alloca(size), CLOX_ALLOCA_ERROR_MESSAGE)
#   endif
#endif

#ifndef freea
#   if CLOX_PLATFORM_IS_WINDOWS
/**
 * @brief       Frees a block of memory allocated on the stack (or the heap) by
 *              malloca.
 * 
 * @param       block The previously allocated mempry block.
 */
#       define freea(block) _freea(block)
#   else
/**
 * @brief       Frees a block of memory allocated on the stack (or the heap) by
 *              malloca.
 * 
 * @param       block The previously allocated mempry block.
 */
#       define freea(block)
#   endif
#endif

#ifndef stackalloc
/**
 * @brief       Allocates a block of bytes of the same dimension of the specified
 *              data type on the stack.
 * 
 * @param       T The type to allocate.
 * @return      A pointer to the new block of memory.
 */
#   define stackalloc(T) (T *)malloca(sizeof(T))
#endif

#ifndef calloca
/**
 * @brief       Allocates a series of block of bytes on the stack, checking not
 *              nullity of them and failing in that case.
 * 
 * @param       count The number of blocks to allocate.
 * @param       size The number of bytes to allocate.
 * @return      A pointer to the new block of memory.
 */
#   define calloca(count, size) malloca(count * size)
#endif

#ifndef stackdim
/**
 * @brief       Allocates a series of N contiguous blocks of memory of the same
 *              dimension of the specified data type on the stack.
 * 
 * @param       T The type to allocate.
 * @param       N The number of items to allocate.
 * @return      A pointer to the new block of memory.
 */
#   define stackdim(T, N) (T *)calloca((N), sizeof(T))
#endif

#ifndef alignto
/**
 * @brief       Gets an aligned number.
 */
#   define alignto(size, alignment) ((size + alignment - 1) & ~(alignment - 1))
#endif

CLOX_C_HEADER_END

#endif /* CLOX_BASE_ALLOC_H_ */
