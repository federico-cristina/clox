#pragma once

/**
 * @file        defs.h
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
 * @brief       In this header are defined commonly used constants and macros
 *              in the whole project.
 */

#ifndef CLOX_BASE_DEFS_H_
#define CLOX_BASE_DEFS_H_

#ifndef _CRT_SECURE_NO_WARNINGS
/**
 * @brief       Disable CRT standard C library functions deprecations. (This
 *              constant avoids errors and warnings while compiling with MSVC
 *              compiler, or clang-cl on Windows)
 */
#   define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <stddef.h>

#ifndef CLOX_C_HEADER_BEGIN
#   ifdef __cplusplus
#       if   defined _CRT_BEGIN_C_HEADER
/**
 * @brief       Marks the beginning of a C header file for C++ compilers.
 */
#           define CLOX_C_HEADER_BEGIN _CRT_BEGIN_C_HEADER
#       elif defined __cplusplus
/**
 * @brief       Marks the beginning of a C header file for C++ compilers.
 */
#           define CLOX_C_HEADER_BEGIN extern "C" {
#       else
/**
 * @brief       Marks the beginning of a C header file for C++ compilers.
 */
#           define CLOX_C_HEADER_BEGIN
#       endif
#   else
/**
 * @brief       Marks the beginning of a C header file for C++ compilers.
 */
#       define CLOX_C_HEADER_BEGIN
#   endif
#endif

#ifndef CLOX_C_HEADER_END
#   ifdef __cplusplus
#       if   defined _CRT_END_C_HEADER
/**
 * @brief       Marks the end of a C header file for C++ compilers.
 */
#           define CLOX_C_HEADER_END _CRT_END_C_HEADER
#       elif defined __cplusplus
/**
 * @brief       Marks the end of a C header file for C++ compilers.
 */
#           define CLOX_C_HEADER_END }
#       else
/**
 * @brief       Marks the end of a C header file for C++ compilers.
 */
#           define CLOX_C_HEADER_END
#       endif
#   else
/**
 * @brief       Marks the end of a C header file for C++ compilers.
 */
#       define CLOX_C_HEADER_END
#   endif
#endif

#include "clox/config.h"

#ifndef cloxExpandMacro
/**
 * @brief       This macro gets its parameter back, expanding it if it's a
 *              symbolic constant.
 *
 * @param       x       The value to get back.
 * @return      The value of the `x` parameter.
 */
#   define cloxExpandMacro(x) x
#endif

#ifndef cloxConcatMacro
/**
 * @brief       This macro concatenates the given tokens, if their constants
 *              they will be expanded.
 *
 * @param       x Prefix token sequence.
 * @param       y Suffix token sequence.
 * @return      The concatenated token sequence.
 */
#   define cloxConcatMacro(x, y) cloxExpandMacro(x) ## cloxExpandMacro(y)
#endif

#ifndef clox_MacroToString
/**
 * @brief       This macro gets a string version of the parameter token.
 * @attention   THIS IS ONLY AN HELPER MACRO DON'T USE IT DIRECTLY.
 */
#   define clox_MacroToString(x) #x
#endif

#ifndef cloxMacroToString
/**
 * @brief       This macro gets a string version of the given token sequence.
 *
 * @param       x Token sequence to stringify.
 * @return      A string version of the `x` parameter.
 */
#   define cloxMacroToString(x) clox_MacroToString(x)
#endif

/**
 * @defgroup    CALLING_CONVENTIONS Calling Conventions
 * @{
 */

#pragma region Calling Conventions

#ifndef CLOX_STDCALL
#   if CLOX_PLATFORM_IS_WINDOWS && CLOX_COMPILER_HAS_CCONVS
/**
 * @brief       This constant provides __stdcall calling convention on systems
 *              and compilers that supports it.
 */
#       define CLOX_STDCALL __stdcall
#   else
/**
 * @brief       This constant provides __stdcall calling convention on systems
 *              and compilers that supports it.
 */
#       define CLOX_STDCALL 
#   endif
#endif

#ifndef CLOX_CDECL
#   if CLOX_PLATFORM_IS_WINDOWS && CLOX_COMPILER_HAS_CCONVS
/**
 * @brief       This constant provides __cdecl calling convention on systems
 *              and compilers that supports it.
 */
#       define CLOX_CDECL __cdecl
#   else
/**
 * @brief       This constant provides __cdecl calling convention on systems
 *              and compilers that supports it.
 */
#       define CLOX_CDECL
#   endif
#endif

#pragma endregion

/**
 * @}
 * 
 * @defgroup    ATTRIBUTES Attributes
 * @{
 */

#pragma region Attributes

#ifndef CLOX_ATTRIBUTE
#   if CLOX_C_EXTENSIONS
#       if CLOX_PLATFORM_IS_WINDOWS && CLOX_COMPILER_HAS_DECLSPEC
/**
 * @brief       This macro provides compiler specific attributes or,
 *              if disabled C extensions, this macro does nothing.
 */
#           define CLOX_ATTRIBUTE(attr) __declspec(attr)
#       else
/**
 * @brief       This macro provides compiler specific attributes or,
 *              if disabled C extensions, this macro does nothing.
 */
#           define CLOX_ATTRIBUTE(attr) __attribute__((attr))
#       endif
#   else
/**
 * @brief       This macro provides compiler specific attributes or,
 *              if disabled C extensions, this macro does nothing.
 */
#       define CLOX_ATTRIBUTE(attr)
#   endif
#endif

#ifndef CLOX_NORETURN
#   if CLOX_C_STANDARD < CLOX_C_STANDARD_C23
/**
 * @brief       This macro provides `noreturn` attribute.
 */
#       define CLOX_NORETURN CLOX_ATTRIBUTE(noreturn)
#   else
/**
 * @brief       This macro provides `noreturn` attribute.
 */
#       define CLOX_NORETURN [[noreturn]]
#   endif
#endif

#ifndef CLOX_DEPRECATED
#   if CLOX_C_STANDARD < CLOX_C_STANDARD_C23
/**
 * @brief       This macro provides `deprecated` attribute.
 */
#       define CLOX_DEPRECATED(reason) CLOX_ATTRIBUTE(deprecated(reason))
#   else
/**
 * @brief       This macro provides `deprecated` attribute.
 */
#       define CLOX_DEPRECATED(reason) [[deparecated(reason)]]
#   endif
#endif

#ifndef CLOX_DLL_EXPORT
#   if CLOX_PLATFORM_IS_WINDOWS
/**
 * @brief       This macro provides compiler specific `dllexport` attribute
 *              to deal with shared library functions exportations, only if
 *              supported.
 */
#       define CLOX_DLL_EXPORT CLOX_ATTRIBUTE(dllexport)
#   elif (CLOX_COMPILER_ID == CLOX_COMPILER_ID_GNUC) && (__GNUC__ >= 4)
/**
 * @brief       This macro provides compiler specific `dllexport` attribute
 *              to deal with shared library functions exportations, only if
 *              supported.
 */
#       define CLOX_DLL_EXPORT CLOX_ATTRIBUTE(visibility("default"))
#   else
/**
 * @brief       This macro provides compiler specific `dllexport` attribute
 *              to deal with shared library functions exportations, only if
 *              supported.
 */
#       define CLOX_DLL_EXPORT
#   endif
#endif

// DLL Attributes

#ifndef CLOX_DLL_IMPORT
#   if CLOX_PLATFORM_IS_WINDOWS
/**
 * @brief       This macro provides compiler specific `dllimport` attribute
 *              to deal with shared library functions importations, only if
 *              supported.
 */
#       define CLOX_DLL_IMPORT CLOX_ATTRIBUTE(dllimport)
#   elif (CLOX_COMPILER_ID == CLOX_COMPILER_ID_GNUC) && (__GNUC__ >= 4)
/**
 * @brief       This macro provides compiler specific `dllimport` attribute
 *              to deal with shared library functions importations, only if
 *              supported.
 */
#       define CLOX_DLL_IMPORT CLOX_ATTRIBUTE(visibility("default"))
#   else
/**
 * @brief       This macro provides compiler specific `dllimport` attribute
 *              to deal with shared library functions importations, only if
 *              supported.
 */
#       define CLOX_DLL_IMPORT
#   endif
#endif

#ifndef CLOX_DLL_STATIC
#   if (CLOX_COMPILER_ID == CLOX_COMPILER_ID_GNUC) && (__GNUC__ >= 4)
/**
 * @brief       This macro makes a function of a shared library be local
 *              in its scope.
 */
#       define CLOX_DLL_STATIC CLOX_ATTRIBUTE(visibility("hidden"))
#   else
/**
 * @brief       This macro makes a function of a shared library be local
 *              in its scope.
 */
#       define CLOX_DLL_STATIC
#   endif
#endif

#pragma endregion

/**
 * @}
 * 
 * @defgroup    C_COMPATIBILITY C Compatibility
 * @{
 */

#pragma region C Compatibility

#ifndef CLOX_REGISTER
#   if !defined __cplusplus && (__cplusplus < 201703L)
/**
 * @brief       This constant provides `register` storage modifier. In C,
 *              hints the compiler to store a variable in a processor
 *              `register` instead of default memory for locals.
 *
 * @note        Since C++17, the `register` keyword, is deprecated and out
 *              of support.
 */
#       define CLOX_REGISTER register
#   else
/**
 * @brief       This constant provides `register` storage modifier. In C,
 *              hints the compiler to store a variable in a processor
 *              `register` instead of default memory for locals.
 *
 * @note        Since C++17, the `register` keyword, is deprecated and out
 *              of support.
 */
#       define CLOX_REGISTER
#   endif
#endif

#ifndef CLOX_INLINE
#   if (CLOX_C_STANDARD >= CLOX_C_STANDARD_C99)
#       if (CLOX_COMPILER_ID == CLOX_COMPILER_ID_GNUC) || CLOX_PLATFORM_IS_UNIX
/**
 * @brief       This constant provides `inline` functions modifier to marks
 *              functions as inlineable. (since C99)
 * 
 * @note        `inline` functions should also be declared as `static` both
 *              in header and source file, so if the `inline` keyword is not
 *              supported, functions denoted by this symbolic constant, are
 *              always `static`. 
 */
#           define CLOX_INLINE static inline
#       else
/**
 * @brief       This constant provides `inline` functions modifier to marks
 *              functions as inlineable. (since C99)
 * 
 * @note        `inline` functions should also be declared as `static` both
 *              in header and source file, so if the `inline` keyword is not
 *              supported, functions denoted by this symbolic constant, are
 *              always `static`. 
 */
#           define CLOX_INLINE inline
#       endif
#   else
/**
 * @brief       This constant provides `inline` functions modifier to marks
 *              functions as inlineable. (since C99)
 * 
 * @note        `inline` functions should also be declared as `static` both
 *              in header and source file, so if the `inline` keyword is not
 *              supported, functions denoted by this symbolic constant, are
 *              always `static`. 
 */
#       define CLOX_INLINE static
#   endif
#endif

#ifndef CLOX_EXTERN
/**
 * @brief       This constants marks a function declaration as `extern`,
 *              so declared out of the current scope.
 */
#   define CLOX_EXTERN extern
#endif

#ifndef CLOX_STATIC
/**
 * @brief       This constants marks a function declaration as `static`,
 *              so visible only in the scope in which is declared.
 */
#   define CLOX_STATIC static
#endif

#pragma endregion

/**
 * @}
 */

#ifndef CLOX_DEBUG
#   if defined DEBUG || defined _DEBUG
/**
 * @brief       This constant provides an internal debug flag.
 */
#       define CLOX_DEBUG 1
#   else
/**
 * @brief       This constant provides an internal debug flag.
 */
#       define CLOX_DEBUG 0
#   endif
#endif

#ifndef CLOX_BUILD_AS_DLL
#   if (defined DLL || defined _DLL) && CLOX_PLATFORM_IS_WINDOWS
/**
 * @brief       This constant provides an internal dll flag.
 */
#       define CLOX_BUILD_AS_DLL 1
#   else
/**
 * @brief       This constant provides an internal dll flag.
 */
#       define CLOX_BUILD_AS_DLL 0
#   endif
#endif

#endif /* CLOX_BASE_DEFS_H_ */
