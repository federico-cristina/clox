#pragma once

/**
 * @file        path.h
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
 *              to manipulate file paths.
 */

#ifndef CLOX_BASE_PATH_H_
#define CLOX_BASE_PATH_H_

#include "clox/base/defs.h"

/**
 * @defgroup    PLATFORM_PATH_DEFINITIONS Platform Path Definitions
 * @{
 */

#pragma region Platform Path Definitions

#if CLOX_PLATFORM_IS_WINDOWS
#   include <direct.h> /* _getcwd, _chdir, ... */

/**
 * @brief       This macro changes the current working directory.
 * 
 * @param       dirname Path of new working directory.
 * @return      These function returns a value of 0 if successful. A return value
 *              of -1 indicates failure.
 */
#   define chdir(dirname)      _chdir((dirname))
/**
 * @brief       The getcwd() macro shall place an absolute pathname of the
 *              current working directory in the array pointed to by dest, and return
 *              dest. The pathname copied to the array shall contain no components
 *              that are symbolic links.
 * 
 * @param       dest Destination buffer on which copy the current working directory.
 * @param       size Is the size in bytes of the character array pointed to by the
 *              dest argument. If dest is a null pointer, the behavior of getcwd()
 *              is unspecified.
 * @return      Upon successful completion, getcwd() shall return the dest argument.
 *              Otherwise, getcwd() shall return a NULL pointer and set errno to
 *              indicate the error. The contents of the array pointed to by buf are
 *              then undefined.
 */
#   define getcwd(dest, size)  _getcwd((dest), (size))

#   ifndef cloxIsDirSep
/**
 * @brief       Checks if a character is a directory separator.
 * 
 * @param       c The character to check.
 * @return      If c is a directory separator then TRUE, else FALSE.
 */
#       define cloxIsDirSep(c)  (c == '/' || c == '\\')
#   endif

#   ifndef cloxIsExtSep
/**
 * @brief       Checks if a character is an extension separator.
 * 
 * @param       c The character to check.
 * @return      If c is a directory separator then TRUE, else FALSE.
 */
#       define cloxIsExtSep(c)  (c == '.')
#   endif

#   ifndef cloxIsAbsPath
/**
 * @brief       Checks if a string represents an absolute path.
 * 
 * @param       s The string to check.
 * @return      If c is a directory separator then TRUE, else FALSE.
 */
#       define cloxIsAbsPath(s) (cloxIsDirSep(*s) || (*s && *(s + 1) == ':' && cloxIsDirSep(*(s + 2))))
#   endif

#   if !CLOX_C_EXTENSIONS
#       include "clox/base/string.h" /* stricmp */
#   endif

#   ifndef CLOX_PATHCMP
/**
 * @brief       This macro contains the name of the path comparator function.
 */
#       define CLOX_PATHCMP     !stricmp
#   endif

#   ifndef CLOX_PATHSEP
/**
 * @brief       This macro contains the path separator character in a path list.
 */
#       define CLOX_PATHSEP     ';'
#   endif
#else
#   include <unistd.h>

#   ifndef cloxIsDirSep
/**
 * @brief       Checks if a character is a directory separator.
 * 
 * @param       c The character to check.
 * @return      If c is a directory separator then TRUE, else FALSE.
 */
#       define cloxIsDirSep(c)  (c == '/')
#   endif

#   ifndef cloxIsExtSep
/**
 * @brief       Checks if a character is an extension separator.
 * 
 * @param       c The character to check.
 * @return      If c is a directory separator then TRUE, else FALSE.
 */
#       define cloxIsExtSep(c)  (c == '.')
#   endif

#   ifndef cloxIsAbsPath
/**
 * @brief       Checks if a string represents an absolute path.
 * 
 * @param       s The string to check.
 * @return      If c is a directory separator then TRUE, else FALSE.
 */
#       define cloxIsAbsPath(s) cloxIsDirSep(*s)
#   endif

#   ifndef CLOX_PATHCMP
/**
 * @brief       This macro contains the name of the path comparator function.
 */
#       define CLOX_PATHCMP     !strcmp
#   endif

#   ifndef CLOX_PATHSEP
/**
 * @brief       This macro contains the path separator character in a path list.
 */
#       define CLOX_PATHSEP     ':'
#   endif
#endif

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_BEGIN

/**
 * @defgroup    PATH_MANIPULATION_FUNCTIONS Path Manipulation Functions
 * @{
 */

#pragma region Path Manipulation Functions

/**
 * @brief       Gets the path's root directory string without checking.
 * 
 * @param       dest A pointer to the destination buffer in which write the root
 *              directory. When NULL the result will be allocated in memory.
 * @param       path Source path from which find the result.
 * @return      A string containing the path's root directory.
 */
CLOX_EXTERN char *CLOX_STDCALL path_getroot(char *const dest, const char *const path);
/**
 * @brief       Gets the file or directory's name string without checking.
 * 
 * @param       dest A pointer to the destination buffer in which write the root
 *              directory. When NULL the result will be allocated in memory.
 * @param       path Source path from which find the result.
 * @return      A string containing the path's root directory.
 */
CLOX_EXTERN char *CLOX_STDCALL path_getname(char *const dest, const char *const path);
/**
 * @brief       Gets the file's extension string without checking.
 * 
 * @param       dest A pointer to the destination buffer in which write the root
 *              directory. When NULL the result will be allocated in memory.
 * @param       path Source path from which find the result.
 * @return      A string containing the path's root directory.
 */
CLOX_EXTERN char *CLOX_STDCALL path_getextn(char *const dest, const char *const path);
/**
 * @brief       Gets the path's base name string without checking.
 * 
 * @param       dest A pointer to the destination buffer in which write the root
 *              directory. When NULL the result will be allocated in memory.
 * @param       path Source path from which find the result.
 * @return      A string containing the path's root directory.
 */
CLOX_EXTERN char *CLOX_STDCALL path_getbase(char *const dest, const char *const path);

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_BASE_PATH_H_ */
