#pragma once

/**
 * @file        errno.h
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
 * @brief       In this header are defined functions to print on the
 *              standard error stream and functions to handle errno
 *              names and messages.
 */

#ifndef CLOX_BASE_ERRNO_H_
#define CLOX_BASE_ERRNO_H_

#include "clox/base/defs.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#pragma warning(disable: 4311)
#pragma warning(disable: 4996)

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    STDERR_FORMATTED_OUTPUT_FUNCTIONS stderr Formatted Output Functions
 * @{
 */

#pragma region stderr Formatted Output Functions

/**
 * @brief       This function prints an error message on the standard error stream.
 *
 * @param       format Pointer to a null-terminated byte string specifying how to
 *              interpret the data.
 * @param       arglist Variable argument list containing the data to print.
 * @return      The number of characters written if successful or negative value
 *              if an error occurred.
 */
CLOX_INLINE int CLOX_STDCALL veprintf(const char *const format, va_list arglist)
{
    return vfprintf(stderr, format, arglist);
}

/**
 * @brief       This function prints an error message on the standard error stream.
 *
 * @param       format Pointer to a null-terminated byte string specifying how to
 *              interpret the data.
 * @param       others Variable argument list containing the data to print.
 * @return      The number of characters written if successful or negative value
 *              if an error occurred.
 */
CLOX_INLINE int CLOX_CDECL eprintf(const char *const format, ...)
{
    int result;
    va_list args;

    va_start(args, format);
    result = veprintf(format, args);
    va_end(args);

    return result;
}

/**
 * @brief       This function prints an error message on the standard error stream
 *              with a line terminator at the end.
 *
 * @param       format Pointer to a null-terminated byte string specifying how to
 *              interpret the data.
 * @param       arglist Variable argument list containing the data to print.
 * @return      The number of characters written if successful or negative value
 *              if an error occurred.
 */
CLOX_INLINE int CLOX_STDCALL veprintfn(const char *const format, va_list arglist)
{
    int result = 0;

    result += vfprintf(stderr, format, arglist);
    result += fputs("\n", stderr);

    return result;
}

/**
 * @brief       This function prints an error message on the standard error stream
 *              with a line terminator at the end.
 *
 * @param       format Pointer to a null-terminated byte string specifying how to
 *              interpret the data.
 * @param       others Variable argument list containing the data to print.
 * @return      The number of characters written if successful or negative value
 *              if an error occurred.
 */
CLOX_INLINE int CLOX_CDECL eprintfn(const char *const format, ...)
{
    int result;
    va_list args;

    va_start(args, format);
    result = veprintfn(format, args);
    va_end(args);

    return result;
}

#pragma endregion

/**
 * @}
 *
 * @defgroup    ERRNO_HANDLING_FUNCTIONS errno Handling Functions
 * @{
 */

#pragma region errno Handling Functions

 /**
  * @brief       This function provides maps each errno code to its name (or
  *              associated macro name).
  *
  * @param       no Errno code.
  * @return      A pointer to the constant string containing the name of the
  *              errno.
  */
CLOX_EXTERN const char *CLOX_STDCALL errnoname(int no);

/**
 * @brief       This function prints on the error stream a message in which is
 *              displayed the errno and its message. (This function has been
 *              created to be used with atexit(...) function)
 */
CLOX_INLINE void CLOX_STDCALL perrno(void)
{
    if (errno)
        eprintfn("errno %d (%s): %s", errno, errnoname(errno), strerror(errno));

    return;
}

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_BASE_ERRNO_H_ */