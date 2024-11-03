/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/path.h"
#include "clox/base/string.h"

CLOX_STATIC size_t CLOX_STDCALL _path_getlastsep(const char *const path, size_t *const outLength)
{
    char *p;
    size_t i, lastSep = 0;

    for (p = (char *)path, i = 0; (*p && (*p != CLOX_PATHSEP)); p++, i++)
    {
        if (cloxIsDirSep(*p))
            lastSep = i + 1;
    }

    if (outLength && *outLength)
        *outLength = i;

    return lastSep;
}

char *CLOX_STDCALL path_getroot(char *const dest, const char *const path)
{
    if (!path)
        return dest;
    else
        return strnmov(dest, path, _path_getlastsep(path, NULL));
}

char *CLOX_STDCALL path_getname(char *const dest, const char *const path)
{
    if (!path)
        return dest;

    size_t lastSep, length;

    lastSep = _path_getlastsep(path, &length);

    return strnmov(dest, (const char *)(path + lastSep), length - lastSep);
}

char *CLOX_STDCALL path_getextn(char *const dest, const char *const path)
{
    if (!path)
        return dest;

    char *p, *result;
    size_t i, lastDot = 0, size;

    for (p = (char *)path, i = 0; (*p && (*p != CLOX_PATHSEP)); p++, i++)
    {
        if (cloxIsExtSep(*p))
            lastDot = i;
    }

    if (!lastDot)
        return NULL;
    else
        return strnmov(dest, (const char *)(path + lastDot), i);
}

char *CLOX_STDCALL path_getbase(char *const dest, const char *const path)
{
    if (!path)
        return dest;

    char *p, *result;
    size_t i, lastSep = 0, lastDot = 0, size;

    for (p = (char *)path, i = 0; (*p && (*p != CLOX_PATHSEP)); p++, i++)
    {
        if (cloxIsDirSep(*p))
            lastSep = i + 1;

        if (cloxIsExtSep(*p))
            lastDot = i;
    }

    return strnmov(dest, (const char *)(path + lastSep), lastDot ? (lastDot - lastSep) : (i - lastSep));
}
