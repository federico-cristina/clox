#pragma once

/**
 * @file        debug.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 */

#ifndef CLOX_VM_DEBUG_H_
#define CLOX_VM_DEBUG_H_

#include "clox/vm/chunk.h"

#include <stdio.h>

CLOX_C_HEADER_BEGIN

CLOX_API void CLOX_STDCALL cloxDisassembleChunk(FILE *const stream, const CloxChunk_t *const chunk);

CLOX_C_HEADER_END

#endif /* CLOX_VM_DEBUG_H_ */
