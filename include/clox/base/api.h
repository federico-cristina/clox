#pragma once

/**
 * @file        api.h
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
 * @brief       In this header are defined utility macro for API constructions
 *              and use.
 */

#ifndef CLOX_BASE_API_H_
#define CLOX_BASE_API_H_

#include "clox/base/defs.h"

#ifndef CLOX_API_PUBLIC
#   if !defined CLOX_NO_DLL && !defined _NO_DLL
#       if CLOX_BUILD_AS_DLL
/**
 * @brief       This constant marks a function as visible at the extern
 *              of the API in which is declared.
 */
#           define CLOX_API_PUBLIC CLOX_DLL_EXPORT
#       else
/**
 * @brief       This constant marks a function as visible at the extern
 *              of the API in which is declared.
 */
#           define CLOX_API_PUBLIC CLOX_DLL_IMPORT
#       endif
#   else
/**
 * @brief       This constant marks a function as visible at the extern
 *              of the API in which is declared.
 */
#       define CLOX_API_PUBLIC
#   endif
#endif

#ifndef CLOX_API_INLINE
#   if CLOX_COMPILER_HAS_DECLSPEC
/**
 * @brief       This constant marks an inline function as visible at the
 *              extern of the API in which is declared, may be not supported.
 */
#       define CLOX_API_INLINE CLOX_API_PUBLIC CLOX_INLINE
#   else
/**
 * @brief       This constant marks an inline function as visible at the
 *              extern of the API in which is declared, may be not supported.
 */
#       define CLOX_API_INLINE CLOX_INLINE
#   endif
#endif

#ifndef CLOX_API_STATIC
/**
 * @brief       This constant marks a function as visible only in the
 *              scope of the API in which is declared.
 */
#   define CLOX_API_STATIC CLOX_DLL_STATIC
#endif

#ifndef CLOX_API
/**
 * @brief       This constant marks a function as part of the current
 *              API.
 */
#   define CLOX_API CLOX_API_PUBLIC
#endif

#endif /* CLOX_BASE_API_H_ */
