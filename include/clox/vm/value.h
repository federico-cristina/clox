#pragma once

/**
 * @file        value.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 * 
 * @brief       In this header is defined CloxValue_t data structure, designed
 *              as a container to store values and data with their type trace.
 * 
 *              In this header are also defined datatypes used to manipulate
 *              values' data and their limit constants.
 */

#ifndef CLOX_VM_VALUE_H_
#define CLOX_VM_VALUE_H_

#include "clox/base/api.h"
#include "clox/base/bits.h"
#include "clox/base/bool.h"
#include "clox/base/byte.h"

#include <stdio.h>

CLOX_C_HEADER_BEGIN

/**
 * @brief       This datatype provides an unsigned integer type used to
 *              represent the unsigned integer value.
 */
typedef uintmax_t uint_t;

#ifndef CLOX_UINT_MIN
/**
 * @brief       This constant provides the minimum value representable with
 *              an uint_t datatype.
 */
#   define CLOX_UINT_MIN UINTMAX_MIN
#endif

#ifndef CLOX_UINT_MAX
/**
 * @brief       This constant provides the maximum value representable with
 *              an uint_t datatype.
 */
#   define CLOX_UINT_MAX UINTMAX_MAX
#endif

#if CLOX_UINT_MAX == UINT32_MAX
/**
 * @brief       This constant provides the width (in bits) of the uint_t
 *              datatype.
 * 
 *              This constant is designed to be used to recognize
 *              when uint_t datatype is 32-bit or 64-bit.
 */
#   define CLOX_UINT_WIDTH 32
#else
/**
 * @brief       This constant provides the width (in bits) of the uint_t
 *              datatype.
 * 
 *              This constant is designed to be used to recognize
 *              when uint_t datatype is 32-bit or 64-bit.
 */
#   define CLOX_UINT_WIDTH 64
#endif

/**
 * @brief       This datatype provides a signed integer datatype used to
 *              represent the signed integer value.
 */
typedef intmax_t sint_t;

#ifndef CLOX_SINT_MIN
/**
 * @brief       This constant provides the minimum value representable with
 *              a sint_t datatype.
 */
#   define CLOX_SINT_MIN INTMAX_MIN
#endif

#ifndef CLOX_SINT_MAX
/**
 * @brief       This constant provides the maximum value representable with
 *              a sint_t datatype.
 */
#   define CLOX_SINT_MAX INTMAX_MAX
#endif

#if CLOX_SINT_MAX == INT32_MAX
/**
 * @brief       This constant provides the width (in bits) of the sint_t
 *              datatype.
 * 
 *              This constant is designed to be used to recognize
 *              when sint_t datatype is 32-bit or 64-bit.
 */
#   define CLOX_SINT_WIDTH 32
#else
/**
 * @brief       This constant provides the width (in bits) of the sint_t
 *              datatype.
 * 
 *              This constant is designed to be used to recognize
 *              when sint_t datatype is 32-bit or 64-bit.
 */
#   define CLOX_SINT_WIDTH 64
#endif

/**
 * @brief       This datatype provides a long double precision floating
 *              point number used to represent real values.
 */
typedef long double real_t;

#ifndef CLOX_REAL_MIN
/**
 * @brief       This constant represents the minimum value representable
 *              with the real_t datatype.
 */
#   define CLOX_REAL_MIN LDBL_MIN
#endif

#ifndef CLOX_REAL_MAX
/**
 * @brief       This constant represents the maximum value representable
 *              with the real_t datatype.
 */
#   define CLOX_REAL_MAX LDBL_MAX
#endif

/**
 * @brief       This datatype provides a raw pointer used to represent
 *              void pointer values.
 */
typedef void *vptr_t;

#ifndef CLOX_VPTR_NULL
/**
 * @brief       This constant provides the NULL constant value represented
 *              with the vptr_t type.
 */
#   define CLOX_VPTR_NULL ((vptr_t)NULL)
#endif

/**
 * @brief       This datatype represents an integral pointer used with
 *              internal purposes to performs safely arithmetic operations
 *              on value pointers.
 */
typedef uintptr_t iptr_t;

#ifndef CLOX_IPTR_MIN
/**
 * @brief       This constant represents the minimum representable value
 *              with a iptr_t type.
 */
#   define CLOX_IPTR_MIN UINTPTR_MIN
#endif

#ifndef CLOX_IPTR_MAX
/**
 * @brief       This constant represents the maximum representable value
 *              with a iptr_t type.
 */
#   define CLOX_IPTR_MAX UINTPTR_MAX
#endif

/**
 * @defgroup    VALUE_TYPE Value Type
 * @{
 */

#pragma region Value Type

typedef enum _CloxValueFlag
{
    CLOX_VALUE_FLAG_NOFLAGS = 0x0000,
    CLOX_VALUE_FLAG_FMTABLE = 0x0100,
    CLOX_VALUE_FLAG_LOGICAL = 0x1000,
    CLOX_VALUE_FLAG_NUMERIC = 0x2000,
    CLOX_VALUE_FLAG_POINTER = 0x4000,
} CloxValueFlag_t;

/**
 * @brief       This enumeration enumerates value types codes, used to
 *              distinguish which operations perform on values
 */
typedef enum _CloxValueType
{
    /**
     * @brief   Represents value without a specified value type.
     * 
     * @note    Void type can be also called 'any', in operations this
     *          datatype is considered as neuter, so the resulting type
     *          will be always other operand's type.
     */
    CLOX_VALUE_TYPE_VOID = 0x00 | CLOX_VALUE_FLAG_NOFLAGS,
    /**
     * @brief   Represents 'bool' values, so values that can assume
     *          only 'true' or 'false' as values and on which can be
     *          performed efficently Boolean algebra.
     * 
     * @note    Boolean type can also be used to represent bits, so
     *          arrays of this type will be implemented as packable.
     */
    CLOX_VALUE_TYPE_BOOL = 0x01 | CLOX_VALUE_FLAG_LOGICAL,
    /**
     * @brief   Represents 'byte' or 'char' values, so values that can
     *          assume a value in a range between 0 and 255.
     */
    CLOX_VALUE_TYPE_BYTE = 0x02 | CLOX_VALUE_FLAG_NUMERIC
                                | CLOX_VALUE_FLAG_FMTABLE,
    /**
     * @brief   Represents 'unsigned' or 'uint' values, so values that
     *          can only store unsigned integers as value (with unspecified
     *          precision).
     */
    CLOX_VALUE_TYPE_UINT = 0x03 | CLOX_VALUE_FLAG_NUMERIC
                                | CLOX_VALUE_FLAG_FMTABLE,
    /**
     * @brief   Represents 'signed' or 'int' values, so values that can
     *          only store signed integers as value (with unspecified
     *          precision).
     */
    CLOX_VALUE_TYPE_SINT = 0x04 | CLOX_VALUE_FLAG_NUMERIC
                                | CLOX_VALUE_FLAG_FMTABLE,
    /**
     * @brief   Represents 'real' values, so values that can only store
     *          real number as long doubled precision floating point
     *          numbers.
     */
    CLOX_VALUE_TYPE_REAL = 0x05 | CLOX_VALUE_FLAG_NUMERIC
                                | CLOX_VALUE_FLAG_FMTABLE,
    /**
     * @brief   Represents raw pointer or 'ptr' values, so values that
     *          can store only a raw pointer (an address of memory).
     */
    CLOX_VALUE_TYPE_VPTR = 0x06 | CLOX_VALUE_FLAG_POINTER,
} CloxValueType_t;

#ifndef cloxValueTypeBoolToString
/**
 * @brief       This macro evaluates a Boolean expression and gets its
 *              value as a string representation.
 */
#   define cloxValueTypeBoolToString(value) ((value) ? "true" : "false")
#endif

/**
 * @defgroup    VALUE_TYPE_FORMATS Value Type Formats
 * @{
 */

#ifndef CLOX_VALUE_TYPE_BOOL_FORMAT
/**
 * @brief       This constant represents the Boolean value type printf
 *              style format.
 */
#   define CLOX_VALUE_TYPE_BOOL_FORMAT      NULL
#endif

#ifndef CLOX_VALUE_TYPE_BYTE_FORMAT
/**
 * @brief       This constant represents the byte value type printf
 *              style format.
 */
#   define CLOX_VALUE_TYPE_BYTE_FORMAT      "%02X"
#endif

#ifndef CLOX_VALUE_TYPE_UINT_FORMAT
#   if   CLOX_UINT_WIDTH == 32
/**
 * @brief       This constant represents the unsigned value type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_UINT_FORMAT  "%lu"
#   elif CLOX_UINT_WIDTH == 64
/**
 * @brief       This constant represents the unsigned value type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_UINT_FORMAT  "%llu"
#   else
/**
 * @brief       This constant represents the unsigned value type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_UINT_FORMAT  "%u"
#   endif
#endif

#ifndef CLOX_VALUE_TYPE_SINT_FORMAT
#   if   CLOX_SINT_WIDTH == 32
/**
 * @brief       This constant represents the integer value type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_SINT_FORMAT  "%ld"
#   elif CLOX_SINT_WIDTH == 64
/**
 * @brief       This constant represents the integer value type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_SINT_FORMAT  "%lld"
#   else
/**
 * @brief       This constant represents the integer value type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_SINT_FORMAT  "%d"
#   endif
#endif

#ifndef CLOX_VALUE_TYPE_REAL_FORMAT
/**
 * @brief       This constant represents the real value type printf
 *              style format.
 */
#   define CLOX_VALUE_TYPE_REAL_FORMAT      "%Lg"
#endif

#ifndef CLOX_VALUE_TYPE_PNTR_FORMAT
#   if CLOX_ARCHTECT_IS_64_BIT
/**
 * @brief       This constant represents the pointer value type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_PNTR_FORMAT      "0x%16p"
#   else
/**
 * @brief       This constant represents the void pointer type printf
 *              style format.
 */
#       define CLOX_VALUE_TYPE_PNTR_FORMAT      "0x%8p"
#   endif
#endif

/**
 * @}
 */

#pragma endregion

/**
 * @}
 * 
 * @defgroup    VALUE_DATA Value Data
 * @{
 */

#pragma region Value Data

/**
 * @brief       This union provides an unique location to store each
 *              possible data value.
 */
typedef union _CloxValueData
{
    /**
     * @brief   Boolean value.
     */
    bool_t Bool;
    /**
     * @brief   Byte value.
     */
    byte_t byte;
    /**
     * @brief   Unsigned integer value.
     */
    uint_t uInt;
    /**
     * @brief   Signed integer value.
     */
    sint_t sInt;
    /**
     * @brief   Real value.
     */
    real_t real;
    /**
     * @brief   Void pointer value.
     */
    vptr_t vPtr;
    /**
     * @brief   Integral pointer value.
     */
    iptr_t iPtr;
} CloxValueData_t;

#ifndef cloxBoolValueData
#   define cloxBoolValueData(value) \
    (CloxValueData_t) {             \
        .Bool = asBool(value)       \
    }
#endif

#ifndef cloxByteValueData
#   define cloxByteValueData(value) \
    (CloxValueData_t) {             \
        .byte = (value)             \
    }
#endif

#ifndef cloxUIntValueData
#   define cloxUIntValueData(value) \
    (CloxValueData_t) {             \
        .uInt = (value)             \
    }
#endif

#ifndef cloxSIntValueData
#   define cloxSIntValueData(value) \
    (CloxValueData_t) {             \
        .sInt = (value)             \
    }
#endif

#ifndef cloxRealValueData
#   define cloxRealValueData(value) \
    (CloxValueData_t) {             \
        .real = (value)             \
    }
#endif

#ifndef cloxVPtrValueData
#   define cloxVPtrValueData(value) \
    (CloxValueData_t) {             \
        .vPtr = (value)             \
    }
#endif

#ifndef cloxIPtrValueData
#   define cloxIPtrValueData(value) \
    (CloxValueData_t) {             \
        .iPtr = (value)             \
    }
#endif

/**
 * @}
 * 
 * @defgroup    VALUE Value
 * @{
 */

#pragma region Value

#if LDBL_DIG == DBL_DIG
/**
 * @brief       This data type provides integral unsigned values to store
 *              the size of a value.
 */
typedef uint16_t CloxValueSize_t;
#else
/**
 * @brief       This data type provides integral unsigned values to store
 *              the size of a value.
 */
typedef uint32_t CloxValueSize_t;
#endif

/**
 * @brief       This data structure provides a container in which store
 *              values during execution and compilation.
 */
typedef struct _CloxValue
{
    /**
     * @brief   Represents the type of the value data wrapped into this
     *          structure.
     */
    CloxValueType_t type;
    /**
     * @brief   Represents the size (in bytes) of the data wrapped into
     *          this structure.
     */
    CloxValueSize_t size;
    /**
     * @brief   Represents data stored into this structure.
     */
    CloxValueData_t data;
} CloxValue_t;

#ifndef cloxValue
#   define cloxValue(valueType, valueData) \
    (CloxValue_t) {                        \
        .type = (valueType),               \
        .size = sizeof(valueData),         \
        .data = (valueData),               \
    }
#endif

#ifndef cloxValueWithSize
#   define cloxValueWithSize(valueType, valueSize, valueData) \
    (CloxValue_t) {                                           \
        .type = (valueType),                                  \
        .size = (valueSize),                                  \
        .data = (valueData),                                  \
    }
#endif

/**
 * @brief       This function intializes a CloxValue_t instance of a specific
 *              type, of a specific size (in bytes) and with a specified initial
 *              value.
 * 
 * @param       value A pointer to the CloxValue_t instance to initialize, or
 *              set.
 * @param       valueType The value type of the data. 
 * @param       valueSize The number of bytes on which is represented the value's
 *              data.
 * @param       valueData Value data to use to initialize the CloxValue_t instance.
 * @return      On success this function returns a pointer to the initialized
 *              CloxValue_t instance (so value parameter).
 */
CLOX_INLINE CloxValue_t *CLOX_STDCALL cloxInitValue(CloxValue_t *const value, const CloxValueType_t valueType, const CloxValueSize_t valueSize, const CloxValueData_t valueData)
{
    assert(value != NULL);

    value->type = valueType;
    value->size = valueSize;
    value->data = valueData;

    return value;
}
/**
 * @brief       This function releases resources used by a specified instance of
 *              CloxValue_t type.
 * 
 * @note        Currently this function doesn't do its work, it will be done when
 *              objects will be implemented.
 * 
 * @param       value A pointer to the CloxValue_t instance to free.
 * @return      On success this function returns a pointer to the initialized
 *              CloxValue_t instance (so value parameter).
 */
CLOX_INLINE CloxValue_t *CLOX_STDCALL cloxFreeValue(CloxValue_t *const value)
{
    assert(value != NULL);

    value->type = CLOX_VALUE_TYPE_VOID;
    value->data.vPtr = CLOX_VPTR_NULL;

    return value;
}

#ifndef cloxBoolValue
#   define cloxBoolValue(value) cloxValueWithSize(CLOX_VALUE_TYPE_BOOL, sizeof(bool_t), cloxBoolValueData(value))
#endif

#ifndef cloxByteValue
#   define cloxByteValue(value) cloxValueWithSize(CLOX_VALUE_TYPE_BYTE, sizeof(byte_t), cloxByteValueData(value))
#endif

#ifndef cloxUIntValue
#   define cloxUIntValue(value) cloxValueWithSize(CLOX_VALUE_TYPE_UINT, sizeof(uint_t), cloxUIntValueData(value))
#endif

#ifndef cloxSIntValue
#   define cloxSIntValue(value) cloxValueWithSize(CLOX_VALUE_TYPE_SINT, sizeof(sint_t), cloxSIntValueData(value))
#endif

#ifndef cloxRealValue
#   define cloxRealValue(value) cloxValueWithSize(CLOX_VALUE_TYPE_REAL, sizeof(real_t), cloxRealValueData(value))
#endif

#ifndef cloxVPtrValue
#   define cloxVPtrValue(value) cloxValueWithSize(CLOX_VALUE_TYPE_VPTR, sizeof(vptr_t), cloxVPtrValueData(value))
#endif

/**
 * @brief       This function dumps a textual (and programmer friendly)
 *              representation of the specified instance of CloxValue_t type
 *              on the specified FILE stream.
 * 
 * @param       stream FILE stream on which write.
 * @param       value A pointer to the CloxValue_t instance to display. 
 * @return      On success this function returns the number of characters written,
 *              and on failure it returns minus one.
 */
CLOX_API int CLOX_STDCALL cloxDumpValue(FILE *const stream, const CloxValue_t *const value);

#pragma endregion

/**
 * @}
 */

CLOX_C_HEADER_END

#endif /* CLOX_VM_VALUE_H_ */
