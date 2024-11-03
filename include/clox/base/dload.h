#pragma once

/**
 * @file        dload.h
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
 * @brief       In this header are defined functions to load dynamically
 *              symbols and shared modules.
 */

#ifndef CLOX_BASE_DLOAD_H_
#define CLOX_BASE_DLOAD_H_

#include "clox/base/bool.h"

CLOX_C_HEADER_BEGIN

/**
 * @brief       Handle datatype.
 */
typedef void *handle_t;

/**
 * @brief       Loads the handle for the current module.
 * 
 * @return      An opaque handle to the current module. 
 */
CLOX_EXTERN handle_t CLOX_STDCALL dlload_current(void);
/**
 * @brief       Loads the specified dynamic (shared) library.
 * 
 * @param       path The path to or the name of the dynamic library to load.
 * @return      An opaque handle to the specified module. 
 */
CLOX_EXTERN handle_t CLOX_STDCALL dlload(const char *const path);

/**
 * @brief       Imports a function from the module pointed by the specified
 *              handle by its name.
 * 
 * @param       handle The handle from which load the function.
 * @param       name The name of the function to import.
 * @return      An opaque handle to the imported function. 
 */
CLOX_EXTERN handle_t CLOX_STDCALL dlimpf(handle_t handle, const char *const name);

/**
 * @brief       Unloads the dynamic (shared) library pointed by the handle
 *              from the memory.
 * 
 * @param       handle The handle of the module to unload.
 * @return      TRUE in case of success, else FALSE. 
 */
CLOX_EXTERN bool_t CLOX_STDCALL dlunload(handle_t handle);

CLOX_C_HEADER_END

#endif /* CLOX_BASE_DLOAD_H_ */
