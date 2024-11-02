#pragma once

/**
 * @file        source_location.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 *
 * @brief       In this header are defined structures and functions to
 *              manipulate source locations.
 */

#ifndef CLOX_SOURCE_SOURCE_LOCATION_H_
#define CLOX_SOURCE_SOURCE_LOCATION_H_

#include "clox/base/api.h"
#include "clox/base/bits.h"

#include <assert.h>

CLOX_C_HEADER_BEGIN

/**
 * @brief       Source location data structure.
 */
typedef struct _CloxSourceLocation
{
    /**
     * @brief   Current character number in the source stream.
     */
    uint64_t ch;
    /**
     * @brief   Current column number in the source stream.
     */
    uint32_t co;
    /**
     * @brief   Current line Number in the source stream.
     */
    uint32_t ln;
} CloxSourceLocation_t;

/**
 * @brief       Sets the values of a source location.
 * 
 * @param       sourceLocation The source location to set.
 * @param       ch The number of the character in the source buffer.
 * @param       co The number of the column in the source buffer.
 * @param       ln The number of the line int the source buffer.
 * @return      This function returns the sourceLocation parameter.
 */
CLOX_API_INLINE CloxSourceLocation_t *CLOX_STDCALL cloxSetSourceLocation(CloxSourceLocation_t *const sourceLocation, uint64_t ch, uint32_t co, uint32_t ln)
{
    assert(sourceLocation != NULL);

    sourceLocation->ch = ch;
    sourceLocation->co = co;
    sourceLocation->ln = ln;

    return sourceLocation;
}

/**
 * @brief       Resets to the zero value the position pointed by a source location.
 * 
 * @param       sourceLocation A pointer to the source location to reset.
 * @return      This function returns the sourceLocation parameter.
 */
CLOX_API_INLINE CloxSourceLocation_t *CLOX_STDCALL cloxResetSourceLocation(CloxSourceLocation_t *const sourceLocation)
{
    return cloxSetSourceLocation(sourceLocation, 0, 0, 0);
}

CLOX_C_HEADER_END

#endif /* CLOX_SOURCE_SOURCE_LOCATION_H_ */
