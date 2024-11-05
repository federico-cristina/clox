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
#include <stdlib.h>
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
 *
 * @defgroup    FATAL_ERRORS_HANDLING Fatal Errors Handling
 * @{
 */

#pragma region Fatal Errors Handling

#ifndef CLOX_ERROR_TRACE_SPACES
/**
 * @brief       This constant represents the default number of spaces used at the
 *              beginning of an error trace. (It can be used to build error messages)
 */
#   define CLOX_ERROR_TRACE_SPACES "    "
#endif

#ifndef CLOX_ERROR_TRACE_STRING
#   if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This constant represents the default internal error trace string used
 *              (in fail macros) to show in which file and in which function has been
 *              thrown the error.
 */
#       define CLOX_ERROR_TRACE_STRING "\n" CLOX_ERROR_TRACE_SPACES "at %s in %s:%lld"
#   else
/**
 * @brief       This constant represents the default internal error trace string used
 *              (in fail macros) to show in which file and in which function has been
 *              thrown the error.
 */
#       define CLOX_ERROR_TRACE_STRING "\n" CLOX_ERROR_TRACE_SPACES "in %s:%ld"
#   endif
#endif

#if CLOX_DEBUG

#if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This function shows an error message supposing that that error is not
 *              fatal.
 * 
 * @param       format Format string of the message to print.
 * @param       others Arguments to use to format the message.
 */
CLOX_INLINE void CLOX_CDECL _warn(const char *const format, const char *const func, const char *const file, long long line, ...)
#else
/**
 * @brief       This function shows an error message supposing that that error is not
 *              fatal.
 * 
 * @param       format Format string of the message to print.
 * @param       others Arguments to use to format the message.
 */
CLOX_INLINE void CLOX_CDECL _warn(const char *const format, const char *const file, long long line, ...)
#endif
{
    va_list arglist;

    va_start(arglist, line);
    veprintf(format, arglist);
    va_end(arglist);

#if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
    eprintfn(CLOX_ERROR_TRACE_STRING, func, file, line);
#else
    eprintfn(CLOX_ERROR_TRACE_STRING, file, line);
#endif

    return;
}

#if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       This function aborts program execution printing a formatted error
 *              message on the error stream.
 * 
 * @param       format Format string of the message to print.
 * @param       others Arguments to use to format the message.
 */
CLOX_INLINE CLOX_NORETURN void CLOX_CDECL _fail(const char *const format, const char *const func, const char *const file, long long line, ...)
#else
/**
 * @brief       This function aborts program execution printing a formatted error
 *              message on the error stream.
 * 
 * @param       format Format string of the message to print.
 * @param       others Arguments to use to format the message.
 */
CLOX_INLINE CLOX_NORETURN void CLOX_CDECL _fail(const char *const format, const char *const file, long long line, ...)
#endif
{
    va_list arglist;

    va_start(arglist, line);
    veprintf(format, arglist);
    va_end(arglist);

#if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
    eprintfn(CLOX_ERROR_TRACE_STRING, func, file, line);
#else
    eprintfn(CLOX_ERROR_TRACE_STRING, file, line);
#endif

    abort();
}

#else

/**
 * @brief       This function shows an error message supposing that that error is not
 *              fatal.
 * 
 * @param       format Format string of the message to print.
 * @param       others Arguments to use to format the message.
 */
CLOX_INLINE void CLOX_CDECL _warn(const char *const format, ...)
{
    va_list arglist;

    va_start(arglist, format);
    veprintf(format, arglist);
    va_end(arglist);

    return;
}

/**
 * @brief       This function aborts program execution printing a formatted error
 *              message on the error stream.
 * 
 * @param       format Format string of the message to print.
 * @param       others Arguments to use to format the message.
 */
CLOX_INLINE CLOX_NORETURN void CLOX_CDECL _fail(const char *const format, ...)
{
    va_list arglist;

    va_start(arglist, format);
    veprintfn(format, arglist);
    va_end(arglist);

    abort();
}

#endif

#ifndef warn
#   if CLOX_DEBUG
#       if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       While debugging reports a non fatal error with the position into
 *              the source code.
 * 
 * @param       format A constant string defining the format of the error message
 *              to print.
 * @param       others The arguments to use to format.
 */
#           define warn(format, ...) _warn(format, __func__, __FILE__, __LINE__, __VA_ARGS__)
#       else
/**
 * @brief       While debugging reports a non fatal error with the position into
 *              the source code.
 * 
 * @param       format A constant string defining the format of the error message
 *              to print.
 * @param       others The arguments to use to format.
 */
#           define warn(format, ...) _warn(format, __FILE__, __LINE__, __VA_ARGS__)
#       endif
#   else
/**
 * @brief       While debugging reports a non fatal error with the position into
 *              the source code.
 * 
 * @param       format A constant string defining the format of the error message
 *              to print.
 * @param       others The arguments to use to format.
 */
#       define warn(format, ...) _warn(format, __VA_ARGS__)
#   endif
#endif

#ifndef fail
#   if CLOX_DEBUG
#       if CLOX_C_STANDARD >= CLOX_C_STANDARD_C99
/**
 * @brief       While debugging aborts printing the name of function from where
 *              has been reported the fatal error.
 * 
 * @param       format A constant string defining the format of the error message
 *              to print.
 * @param       others The arguments to use to format.
 */
#           define fail(format, ...) _fail(format, __func__, __FILE__, __LINE__, __VA_ARGS__)
#       else
/**
 * @brief       While debugging aborts printing the name of function from where
 *              has been reported the fatal error.
 * 
 * @param       format A constant string defining the format of the error message
 *              to print.
 * @param       others The arguments to use to format.
 */
#           define fail(format, ...) _fail(format, __FILE__, __LINE__, __VA_ARGS__)
#       endif
#   else
/**
 * @brief       While debugging aborts printing the name of function from where
 *              has been reported the fatal error.
 * 
 * @param       format A constant string defining the format of the error message
 *              to print.
 * @param       others The arguments to use to format.
 */
#       define fail(format, ...) _fail(format, __VA_ARGS__)
#   endif
#endif

#ifndef notice
/**
 * @brief       Reports an error.
 * 
 * @param       essage An additional message to print to explain the error.
 */
#   define notice(message) (warn("error: %s", (message)), NULL)
#endif

#ifndef failno
/**
 * @brief       Aborts program execution printing an error message based on
 *              the current value of errno.
 * 
 * @param       essage An additional message to print to explain the error.
 */
#   define failno(message) (fail("errno %d (%s): %s (%s)", errno, errnoname(errno), (message), strerror(errno)), NULL)
#endif

#ifndef unreach
/**
 * @brief       This macro marks a portion of code which normally cannot be
 *              reached without some errors. So, when unreachable code is
 *              reached, there must be an error (maybe uknown) and so fails.
 */
#   define unreach() (fail("error: unreachable code has been reached", NULL), NULL)
#endif

#ifndef noimpl
/**
 * @brief       This macro marks a portion of code not yet implemented and so
 *              fails when reach it.
 */
#   define notimpl() (fail("error: not implemented yet", NULL), NULL)
#endif

/* Common error messages */

#ifndef CLOX_ERROR_MESSAGE_STACK_OVERFLOW
#   define CLOX_ERROR_MESSAGE_STACK_OVERFLOW "stack overflow"
#endif

#ifndef CLOX_ERROR_MESSAGE_STACK_UNDERFLOW
#   define CLOX_ERROR_MESSAGE_STACK_UNDERFLOW "stack underflow"
#endif

#ifndef CLOX_ERROR_MESSAGE_BUFFER_OVERRUN
#   define CLOX_ERROR_MESSAGE_BUFFER_OVERRUN "buffer overrun"
#endif

#ifndef CLOX_ERROR_MESSAGE_BUFFER_UNDERRUN
#   define CLOX_ERROR_MESSAGE_BUFFER_UNDERRUN "buffer underrun"
#endif

#ifndef CLOX_ERROR_MESSAGE_BUFFER_INDEX_OUT_OF_BOUNDS
#   define CLOX_ERROR_MESSAGE_BUFFER_INDEX_OUT_OF_BOUNDS "index out of bounds"
#endif

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_BASE_ERRNO_H_ */
