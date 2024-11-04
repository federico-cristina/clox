#pragma once

/**
 * @file        string.h
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
 *              to allocate, manipulate and operate with strings and
 *              characters.
 */

#ifndef CLOX_BASE_STRING_H_
#define CLOX_BASE_STRING_H_

#include "clox/base/bool.h"
#include "clox/base/alloc.h"

#include <string.h>

CLOX_C_HEADER_BEGIN

#ifndef NUL
/**
 * @brief       This constant represents the string terminator character (NUL).
 */
#   define NUL '\0'
#endif

/**
 * @brief       Checks if the given character is a line termination character.
 * 
 * @param       c Character to check.
 * @return      Non-zero value if the character is a line terminator character,
 *              0 otherwise.
 */
CLOX_INLINE int CLOX_STDCALL isendln(int c)
{
    return (c == '\r') || (c == '\n') || (c == NUL);
}

/**
 * @brief       Checks if the given character is a termination character.
 * 
 * @param       c Character to check.
 * @return      Non-zero value if the character is a line terminator character,
 *              0 otherwise. 
 */
CLOX_INLINE int CLOX_STDCALL istermn(int c)
{
    return (c == NUL) || (c == EOF);
}

#ifndef stralloc
#   if CLOX_PLATFORM_IS_WINDOWS && CLOX_C_EXTENSIONS
/**
 * @brief       Allocates an empty string of a specified number of characters.
 * 
 * @param       length The number of characters of the string.
 * @return      A pointer to the first character of the new allocated string.
 */
#       define stralloc(length) (strset((char *)dim(char, (length) + 1), NUL))
#   else
/**
 * @brief       Allocates an empty string of a specified number of characters.
 * 
 * @param       length The number of characters of the string.
 * @return      A pointer to the first character of the new allocated string.
 */
#       define stralloc(length) ((char *)memset((char *)dim(char, (length) + 1), NUL, (length) + 1))
#   endif
#endif

/**
 * @}
 * 
 * @defgroup    STRING_MANIPULATION_FUNCTIONS String Manipulation Functions
 * @{
 */

#pragma region String Manipulation Functions

#if CLOX_PLATFORM_IS_MACOS
/**
 * @brief       Performs a case-insensitive comparison of strings.
 */
#   define stricmp(str1, str2) strcasecmp((str1), (str2))
#endif

/**
 * @brief       Gets a duplicate of a portion of a string. The name is not strndup
 *              to avoid name collisions with strndup(..) extension function.
 * 
 * @param       str The string to duplicate.
 * @param       count The number of characters to duplicate.
 * @return      A pointer to the first character of the duplicate string.
 */
CLOX_EXTERN char *CLOX_STDCALL strnget(const char *const str, size_t count);
/**
 * @brief       Moves characters from source string into destination buffer. If
 *              destination buffer is NULL is allocated a new buffer with as
 *              character as specified by count parameter.
 * 
 * @param       dest Destination buffer.
 * @param       src Source string.
 * @param       count Number of characters to move.
 * @return      A pointer to the destination buffer or to the first character of
 *              the new allocated buffer.
 */
CLOX_EXTERN char *CLOX_STDCALL strnmov(char *const dest, const char *const src, size_t count);
/**
 * @brief       Converts a source string into its lower-case version and writes
 *              the result into a destination buffer or, if that one is NULL, into
 *              a new allocated buffer.
 * 
 * @param       dest Destination buffer.
 * @param       src Source string.
 * @param       count Number of characters to move.
 * @return      A pointer to the destination buffer or to the first character of
 *              the new allocated buffer.
 */
CLOX_EXTERN char *CLOX_STDCALL strnlow(char *const dest, const char *const src, size_t count);
/**
 * @brief       Converts a source string into its upper-case version and writes
 *              the result into a destination buffer or, if that one is NULL, into
 *              a new allocated buffer.
 * 
 * @param       dest Destination buffer.
 * @param       src Source string.
 * @param       count Number of characters to move.
 * @return      A pointer to the destination buffer or to the first character of
 *              the new allocated buffer.
 */
CLOX_EXTERN char *CLOX_STDCALL strnupp(char *const dest, const char *const src, size_t count);

/**
 * @brief       Gets a duplicate of a string. The name is not strdup to avoid name
 *              collisions with strdup(..) extension function.
 * 
 * @param       str String to duplicate.
 * @return      A pointer to the first character of the duplicate string.
 */
CLOX_INLINE char *CLOX_STDCALL strget(const char *const str)
{
    CLOX_REGISTER char *result;

    if (!str)
        result = stralloc(0);
    else
        result = strnget(str, strlen(str));

    return result;
}

/// @brief Moves characters from source string into destination
///        buffer. If the destination buffer is NULL will be allocated
///        a new buffer.
/// @param dest Destination buffer.
/// @param src Source string.
/// @return A pointer to the destination buffer or to the first
///         character of the new allocated buffer.
CLOX_INLINE char *CLOX_STDCALL strmov(char *const dest, const char *const src)
{
    CLOX_REGISTER size_t length;

    if (!src)
        length = 0;
    else
        length = strlen(src);

    return strnmov(dest, src, length);
}

/// @brief Converts a source string into its lower-case version
///        and writes the result into a destination buffer or, if
///        that one is NULL, into a new allocated buffer.
/// @param dest Destination buffer.
/// @param src Source string.
/// @return A pointer to the destination buffer or to the first
///         character of the new allocated buffer.
CLOX_INLINE char *CLOX_STDCALL strlow(char *const dest, const char *const src)
{
    CLOX_REGISTER size_t length;

    if (!src)
        length = 0;
    else
        length = strlen(src);

    return strnlow(dest, src, length);
}

/// @brief Converts a source string into its upper-case version
///        and writes the result into a destination buffer or, if
///        that one is NULL, into a new allocated buffer.
/// @param dest Destination buffer.
/// @param src Source string.
/// @return A pointer to the destination buffer or to the first
///         character of the new allocated buffer.
CLOX_INLINE char *CLOX_STDCALL strupp(char *const dest, const char *const src)
{
    CLOX_REGISTER size_t length;

    if (!src)
        length = 0;
    else
        length = strlen(src);

    return strnupp(dest, src, length);
}

#pragma endregion

/**
 * @}
 * 
 * @defgroup    STRING_EQUALITY_FUNCTIONS String Equality Functions
 * @{
 */

#pragma region String Equality Functions

#if CLOX_PLATFORM_IS_MACOS
/**
 * @brief       Performs a case-insensitive comparison of strings.
 * 
 * @param       str1 First null-terminated strings to compare.
 * @param       str2 Second null-terminated strings to compare.
 * @return      -1 if str1 is less than str2, +1 if str1 is grater than str2, or 0
 *              if str1 and str2 have the same value.
 */
#   define stricmp(str1, str2) strcasecmp((str1), (str2))
#else
/**
 * @brief       Performs a case-insensitive comparison of strings.
 * 
 * @param       str1 First null-terminated strings to compare.
 * @param       str2 Second null-terminated strings to compare.
 * @return      -1 if str1 is less than str2, +1 if str1 is grater than str2, or 0
 *              if str1 and str2 have the same value.
 */
CLOX_EXTERN int CLOX_STDCALL stricmp(const char *const str1, const char *const str2);
#endif

/**
 * @brief       Compares two strings to check if they're the same or if they have
 *              the same content.
 * 
 * @param       str1 First null-terminated strings to compare.
 * @param       str2 Second null-terminated strings to compare.
 * @return      TRUE, if str1 equals to str2. In the other case will be returned
 *              FALSE.
 */
CLOX_EXTERN bool_t CLOX_STDCALL streq(const char *const str1, const char *const str2);
/**
 * @brief       Compares two strings to check if they're the same or if they have
 *              the same content, ignoring the case of characters.
 * 
 * @param       str1 First null-terminated strings to compare.
 * @param       str2 Second null-terminated strings to compare.
 * @return      TRUE, if str1 equals to str2. In the other case will be returned
 *              FALSE.
 */
CLOX_EXTERN bool_t CLOX_STDCALL striq(const char *const str1, const char *const str2);

#pragma endregion

/**
 * @}
 * 
 * @defgroup    STRING_ARRAY_SORTING_FUNCTIONS String Array Sorting Functions
 * @{
 */

#pragma region String Array Sorting Functions

CLOX_INLINE int CLOX_STDCALL _clox_strcmp(const void *const str1, const void *const str2)
{
    return strcmp(*(const char **)str1, *(const char **)str2);
}

CLOX_INLINE int CLOX_STDCALL _clox_stricmp(const void *const str1, const void *const str2)
{
    return stricmp(*(const char **)str1, *(const char **)str2);
}

/**
 * @brief       Sorts an array of strings using a specific comparer.
 * 
 * @param       a The array of strings to sort.
 * @param       count The number of items in the array.
 * @param       cmp A pointer to the comparer function.
 * @return      A pointer to the sorted array.
 */
CLOX_INLINE const char **CLOX_STDCALL strcsrt(const char **const a, size_t count, int (*cmp)(const void *const, const void *const))
{
    return qsort((void *)a, count, sizeof(*a), cmp), a;
}

/**
 * @brief       Sorts a string array using the default comparer.
 * 
 * @param       a The array of strings to sort.
 * @param       count The number of items in the array.
 * @return      A pointer to the sorted array.
 */
CLOX_INLINE const char **CLOX_STDCALL strsrt(const char **const a, size_t count)
{
    return strcsrt(a, count, _clox_strcmp);
}

/**
 * @brief       Sorts a string array using case insensitive comparer.
 * 
 * @param       a The array of strings to sort.
 * @param       count The number of items in the array.
 * @return      A pointer to the sorted array.
 */
CLOX_INLINE const char **CLOX_STDCALL strisrt(const char **const a, size_t count)
{
    return strcsrt(a, count, _clox_stricmp);
}

#pragma endregion

/**
 * @}
 * 
 * @defgroup    STRING_FORMATTING_FUNCTIONS String Formatting Functions
 * @{
 */

#pragma region String Formatting Functions

/**
 * @brief       Allocates a new formatted string with a va_list, with vprintf
 *              style format.
 * 
 * @param       format printf-style format string. 
 * @param       others Arguments list.
 * @return      A pointer to the new allocated formatted string.
 */
CLOX_INLINE char *CLOX_STDCALL vstrfmt(const char *const format, va_list arglist)
{
    if (!format)
        return NULL;

    int size = vsnprintf(NULL, 0, format, arglist);

    if (!size)
        failno("cannot format an empty string");

    char *result = stralloc(size);

    if (vsprintf(result, format, arglist) != size)
        fail("error: formatted string sizes doesn't match", NULL);

    return result;
}

/**
 * @brief       Allocates a new formatted string with a printf like format.
 * 
 * @param       format printf-style format string. 
 * @param       others Arguments list.
 * @return      A pointer to the new allocated formatted string.
 */
CLOX_INLINE char *CLOX_STDCALL strfmt(const char *const format, ...)
{
    if (!format)
        return NULL;

    char *result;
    va_list args;

    va_start(args, format);
    result = vstrfmt(format, args);
    va_end(args);

    return result;
}

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_BASE_STRING_H_ */
