#pragma once

/**
 * @file        bits.h
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
 * @brief       In this header are defined fixed bit-width datatypes,
 *              their constant limits and also useful macros to deal with
 *              datatype sizes.
 */

#ifndef CLOX_BASE_BITS_H_
#define CLOX_BASE_BITS_H_

#include "clox/base/defs.h"

#include <float.h>
#include <limits.h>

#if CLOX_PLATFORM_IS_UNIX
#   include <unistd.h>
#endif

#ifndef countof
/**
 * @brief       Computes the number of elements in a specified array.
 * 
 * @note        This macro works only if used with arrays and NOT with pointers
 *              NOR pointers to an array.
 * 
 * @param       array The array which is requested the length.
 * @return      The number of elements in the array.
 */
#   define countof(array) (sizeof(array) / sizeof(*array))
#endif

#ifndef CLOX_SIZEOF_CHAR
 /**
  * @brief       This constant represents the number of bytes in a system char
  *              to compute the number of bits in other data types.
  */
#   define CLOX_SIZEOF_CHAR     sizeof(char)
#endif

#ifndef CLOX_SIZEOF_WORD
  /**
   * @brief       This constant represents the number of bytes in a system word
   *              to compute the number of words in other data types.
   */
#   define CLOX_SIZEOF_WORD     sizeof(int)
#endif

#ifndef CLOX_SIZEOF_WORD_PTR
   /**
    * @brief       This constant represents the number of bytes in a system word
    *              pointer.
    */
#   define CLOX_SIZEOF_WORD_PTR sizeof(int*)
#endif

#ifndef CLOX_BITSOF_CHAR
/**
 * @brief       This constant represents the number of bits in a system char
 *              to compute the number of bits in other data types.
 */
#   define CLOX_BITSOF_CHAR     CHAR_BIT
#endif

#ifndef CLOX_BITSOF_WORD
/**
 * @brief       This constant represents the number of bits in a system word
 *              to compute the number of words in other data types.
 */
#   define CLOX_BITSOF_WORD     (CLOX_BITSOF_CHAR * CLOX_SIZEOF_WORD)
#endif

#ifndef CLOX_BITSOF_WORD_PTR
/**
 * @brief       This constant represents the number of bits in a system word
 *              pointer.
 */
#   define CLOX_BITSOF_WORD_PTR (CLOX_BITSOF_CHAR * CLOX_SIZEOF_WORD_PTR)
#endif

#ifndef bitsof
/**
 * @brief       This macro computes the number of bits in a specified data type.
 * 
 * @param       T Specified data type.
 * @return      The number of bits required from a value of type T.
 */
#   define bitsof(T)   ((size_t)(sizeof(T) * CLOX_BITSOF_CHAR))
#endif

#ifndef wordsin
/**
 * @brief       This macro computes the number of words in a specified data type.
 * 
 * @param       T Specified data type.
 * @return      The number of words required from a value of type T.
 */
#   define wordsin(T)  ((size_t)(bitsof(T) / CLOX_BITSOF_WORD))
#endif

#ifndef ldwordsin
/**
 * @brief       This macro computes the number of words in a specified data type.
 * 
 * @param       T Specified data type.
 * @return      The number of words required from a value of type T.
 */
#   define ldwordsin(T) ((long double)bitsof(T) / (long double)CLOX_BITSOF_WORD)
#endif

#ifndef CLOX_PAGESIZ
/**
 * @brief       This constant represents the value to use as page page size,
 *              this value is common on many systems.
 */
#   define CLOX_PAGESIZ 4096
#endif

#ifndef alignto
/**
 * @brief       Gets an aligned number.
 */
#   define alignto(size, alignment) ((size + alignment - 1) & ~(alignment - 1))
#endif

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    INTEGRAL_TYPES Integral Types
 * @{
 */

#pragma region Integral Types

#if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
#   include <stdint.h>
#else

/* Signed integers */

#if !defined INT8_MIN || !defined INT8_MAX

/**
 * @brief       Signed 8-bit integer datatype.
 */
typedef signed char int8_t;

#ifndef INT8_MIN
/**
 * @brief       This macro represents the minimum value representable with an
 *              8-bit signed integer.
 */
#   define INT8_MIN (-0x80)
#endif

#ifndef INT8_MAX
/**
 * @brief       This macro represents the maximum value representable with an
 *              8-bit signed integer.
 */
#   define INT8_MAX (+0x7F)
#endif

#endif

#if !defined INT16_MIN || !defined INT16_MAX

/**
 * @brief       Signed 16-bit integer datatype.
 */
typedef short int16_t;

#ifndef INT16_MIN
/**
 * @brief       This macro represents the minimum value representable with an
 *              32-bit signed integer.
 */
#   define INT16_MIN (-0x8000)
#endif

#ifndef INT16_MAX
/**
 * @brief       This macro represents the maximum value representable with an
 *              16-bit signed integer.
 */
#   define INT16_MAX (+0x7FFF)
#endif

#endif

#if !defined INT32_MIN || !defined INT32_MAX

/**
 * @brief       Signed 32-bit integer datatype.
 */
typedef int int32_t;

#ifndef INT32_MIN
/**
 * @brief       This macro represents the minimum value representable with an
 *              32-bit signed integer.
 */
#   define INT32_MIN (-0x80000000)
#endif

#ifndef INT32_MAX
/**
 * @brief       This macro represents the maximum value representable with an
 *              32-bit signed integer.
 */
#   define INT32_MAX (+0x7FFFFFFF)
#endif

#endif

#if (!defined INT64_MIN || !defined INT64_MAX) || CLOX_PLATFORM_IS_WINDOWS

#if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       Signed 64-bit integer datatype.
 */
typedef long long int64_t;
#else
/**
 * @brief       Signed 64-bit integer datatype.
 */
typedef int32_t int64_t;
#endif

#ifndef INT64_MIN
#   if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This macro represents the minimum value representable with an
 *              64-bit signed integer.
 */
#       define INT64_MIN (-0x8000000000000000)
#   else
/**
 * @brief       This macro represents the minimum value representable with an
 *              64-bit signed integer.
 */
#       define INT64_MIN INT32_MIN
#   endif
#endif

#ifndef INT64_MAX
#   if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This macro represents the maximum value representable with an
 *              64-bit signed integer.
 */
#       define INT64_MAX (+0x7FFFFFFFFFFFFFFF)
#   else
/**
 * @brief       This macro represents the maximum value representable with an
 *              64-bit signed integer.
 */
#       define INT64_MAX INT32_MAX
#   endif
#endif

#endif

#if (!defined INTPTR_MIN || !defined INTPTR_MAX) && !CLOX_PLATFORM_IS_WINDOWS

#if CLOX_ARCHTECT_IS_64_BIT
/**
 * @brief       Signed pointer integer datatype.
 */
typedef int64_t intptr_t;
#else
/**
 * @brief       Signed pointer integer datatype.
 */
typedef int32_t intptr_t;
#endif

#ifndef INTPTR_MIN
#   if CLOX_ARCHTECT_IS_64_BIT
/**
 * @brief       This macro represents the minimum value representable with a
 *              signed integer pointer.
 */
#       define INTPTR_MIN INT64_MIN
#   else
/**
 * @brief       This macro represents the minimum value representable with a
 *              signed integer pointer.
 */
#       define INTPTR_MIN INT32_MIN
#   endif
#endif

#ifndef INTPTR_MAX
#   if CLOX_ARCHTECT_IS_64_BIT
/**
 * @brief       This macro represents the maximum value representable with a
 *              signed integer pointer.
 */
#       define INTPTR_MAX INT64_MAX
#   else
/**
 * @brief       This macro represents the maximum value representable with a
 *              signed integer pointer.
 */
#       define INTPTR_MAX INT32_MAX
#   endif
#endif

#endif

#if !defined INTMAX_MIN || !defined INTMAX_MAX

/**
 * @brief       Signed biggest integer datatype.
 */
typedef int64_t intmax_t;

#ifndef INTMAX_MIN
/**
 * @brief       This macro represents the minimum value representable with a
 *              signed integer of the maximum size.
 */
#   define INTMAX_MIN INT64_MIN
#endif

#ifndef INTMAX_MAX
/**
 * @brief       This macro represents the maximum value representable with a
 *              signed integer of the maximum size.
 */
#   define INTMAX_MAX INT64_MAX
#endif

#endif

/* Unsigned integers */

#if !defined UINT8_MIN || !defined UINT8_MAX

/**
 * @brief       Unsigned 8-bit integer datatype.
 */
typedef unsigned char uint8_t;

#ifndef UINT8_MIN
/**
 * @brief       This macro represents the minimum value representable with an
 *              8-bit unsigned integer.
 */
#   define UINT8_MIN (0x00U)
#endif

#ifndef UINT8_MAX
/**
 * @brief       This macro represents the maximum value representable with an
 *              8-bit unsigned integer.
 */
#   define UINT8_MAX (0xFFU)
#endif

#endif

#if !defined UINT16_MIN || !defined UINT16_MAX

/**
 * @brief       Unsigned 16-bit integer datatype.
 */
typedef unsigned short uint16_t;

#ifndef UINT16_MIN
/**
 * @brief       This macro represents the minimum value representable with an
 *              16-bit unsigned integer.
 */
#   define UINT16_MIN (0x0000U)
#endif

#ifndef UINT16_MAX
/**
 * @brief       This macro represents the maximum value representable with an
 *              16-bit unsigned integer.
 */
#   define UINT16_MAX (0xFFFFU)
#endif

#endif

#if !defined UINT32_MIN || !defined UINT32_MAX

/**
 * @brief       Unsigned 32-bit integer datatype.
 */
typedef unsigned int uint32_t;

#ifndef UINT32_MIN
/**
 * @brief       This macro represents the minimum value representable with an
 *              32-bit unsigned integer.
 */
#   define UINT32_MIN (0x00000000U)
#endif

#ifndef UINT32_MAX
/**
 * @brief       This macro represents the maximum value representable with an
 *              32-bit unsigned integer.
 */
#   define UINT32_MAX (0xFFFFFFFFU)
#endif

#endif

#if (!defined UINT64_MIN || !defined UINT64_MAX)

#if (CLOX_C_STANDARD >= CLOX_C_STANDARD_C99) || (CLOX_PLATFORM_IS_MACOS && CLOX_C_EXTENSIONS)
/**
 * @brief       Unsigned 64-bit integer datatype.
 */
typedef unsigned long long uint64_t;
#else
/**
 * @brief       Unsigned 64-bit integer datatype.
 */
typedef uint32_t uint64_t;
#endif

#ifndef UINT64_MIN
#   if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This macro represents the minimum value representable with an
 *              64-bit unsigned integer.
 */
#       define UINT64_MIN (0x0000000000000000U)
#   else
/**
 * @brief       This macro represents the minimum value representable with an
 *              64-bit unsigned integer.
 */
#       define UINT64_MIN UINT32_MIN
#   endif
#endif

#ifndef UINT64_MAX
#   if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This macro represents the maximum value representable with an
 *              64-bit unsigned integer.
 */
#       define UINT64_MAX (0xFFFFFFFFFFFFFFFFU)
#   else
/**
 * @brief       This macro represents the maximum value representable with an
 *              64-bit unsigned integer.
 */
#       define UINT64_MAX UINT32_MAX
#   endif
#endif

#endif

#if (!defined UINTPTR_MIN || !defined UINTPTR_MAX) && !CLOX_PLATFORM_IS_WINDOWS

#if CLOX_ARCHTECT_IS_64_BIT
/**
 * @brief       Unsigned pointer integer datatype.
 */
typedef uint64_t uintptr_t;
#else
/**
 * @brief       Unsigned pointer integer datatype.
 */
typedef uint32_t uintptr_t;
#endif

#ifndef UINTPTR_MIN
#   if CLOX_ARCHTECT_IS_64_BIT
/**
 * @brief       This macro represents the minimum value representable with an
 *              unsigned integer pointer.
 */
#       define UINTPTR_MIN UINT64_MIN
#   else
/**
 * @brief       This macro represents the minimum value representable with an
 *              unsigned integer pointer.
 */
#       define UINTPTR_MIN UINT32_MIN
#   endif
#endif

#ifndef UINTPTR_MAX
#   if CLOX_ARCHTECT_IS_64_BIT
/**
 * @brief       This macro represents the maximum value representable with an
 *              unsigned integer pointer.
 */
#       define UINTPTR_MAX UINT64_MAX
#   else
/**
 * @brief       This macro represents the maximum value representable with an
 *              unsigned integer pointer.
 */
#       define UINTPTR_MAX UINT32_MAX
#   endif
#endif

#endif

#if !defined UINTMAX_MIN || !defined UINTMAX_MAX

/**
 * @brief       Unsigned biggest integer datatype.
 */
typedef uint64_t uintmax_t;

#ifndef UINTMAX_MIN
/**
 * @brief       This macro represents the minimum value representable with an
 *              unsigned integer of the maximum size.
 */
#   define UINTMAX_MIN UINT64_MIN
#endif

#ifndef UINTMAX_MAX
/**
 * @brief       This macro represents the maximum value representable with an
 *              unsigned integer of the maximum size.
 */
#   define UINTMAX_MAX UINT64_MAX
#endif

#endif

#endif

#if CLOX_PLATFORM_IS_WINDOWS && !defined SSIZE_T

#ifndef SSIZE_T
/**
 * @brief       Signed size type constant.
 */
#   define SSIZE_T long long
#endif

/**
 * @brief       Signed size datatype.
 */
typedef SSIZE_T ssize_t;

#endif

#ifndef SSIZE_MIN
#   ifdef LLONG_MIN
/**
 * @brief       This constant represents the minimum ssize_t value.
 */
#       define SSIZE_MIN LLONG_MIN
#   else
/**
 * @brief       This constant represents the minimum ssize_t value.
 */
#       define SSIZE_MIN LONG_MIN
#   endif
#endif

#ifndef SSIZE_MAX
#   ifdef LLONG_MAX
/**
 * @brief       This constant represents the maximum ssize_t value.
 */
#       define SSIZE_MAX LLONG_MAX
#   else
/**
 * @brief       This constant represents the maximum ssize_t value.
 */
#       define SSIZE_MAX LONG_MAX
#   endif
#endif

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_BASE_BITS_H_ */
