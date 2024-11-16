/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/errno.h"

#include "clox/vm/value.h"

#if defined CLOX_OPTIMIZE && CLOX_OPTIMIZE
        CLOX_STATIC const char *const clox_FormatsTable[] = {
            CLOX_VALUE_TYPE_BYTE_FORMAT,
            CLOX_VALUE_TYPE_UINT_FORMAT,
            CLOX_VALUE_TYPE_SINT_FORMAT,
            CLOX_VALUE_TYPE_REAL_FORMAT,
        };
#endif

CLOX_API int CLOX_STDCALL cloxDumpValue(FILE *const stream, const CloxValue_t *const value)
{
    assert(stream != NULL && value != NULL);

    CLOX_REGISTER int result;

    if (hasflag(value->type, CLOX_VALUE_FLAG_FMTABLE))
    {
        CLOX_REGISTER const char *format = NULL;

#if defined CLOX_OPTIMIZE && CLOX_OPTIMIZE
        CLOX_REGISTER const CloxValueType_t valueType = (value->type - CLOX_VALUE_TYPE_BYTE);

        if (valueType < countof(clox_FormatsTable))
            format = clox_FormatsTable[valueType];
        else
            return -1;
#else
        switch (value->type)
        {
        case CLOX_VALUE_TYPE_BYTE:
            format = CLOX_VALUE_TYPE_BYTE_FORMAT;
            break;

        case CLOX_VALUE_TYPE_UINT:
            format = CLOX_VALUE_TYPE_UINT_FORMAT;
            break;

        case CLOX_VALUE_TYPE_SINT:
            format = CLOX_VALUE_TYPE_SINT_FORMAT;
            break;

        case CLOX_VALUE_TYPE_REAL:
            format = CLOX_VALUE_TYPE_REAL_FORMAT;
            break;

        default:
            return -1;
        }
#endif

        result = fprintf(stream, format, value->data);
    }
    else
    {
        CLOX_REGISTER const CloxValueType_t valueType = value->type;

        switch (valueType)
        {
#if defined CLOX_OPTIMIZE && CLOX_OPTIMIZE
            CLOX_REGISTER const char *str;

        case CLOX_VALUE_TYPE_VOID:
            str = "void";
            goto l_fputs;

        case CLOX_VALUE_TYPE_BOOL:
            str = cloxValueTypeBoolToString(value->data.Bool);
            goto l_fputs;

        l_fputs:
            result = fputs(str, stream);
            break;
#else
        case CLOX_VALUE_TYPE_VOID:
            result = fputs("void", stream);
            break;

        case CLOX_VALUE_TYPE_BOOL:
            result = fputs(cloxValueTypeBoolToString(value->data.Bool), stream);
            break;
#endif

        case CLOX_VALUE_TYPE_VPTR:
            result = fprintf(stream, CLOX_VALUE_TYPE_PNTR_FORMAT, value->data.iPtr);
            break;

        default:
            result = -1; /* in case of erroneus code, this function returns -1 */
            break;
        }
    }

    return result;
}
