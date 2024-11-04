/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/string.h"

#include <ctype.h>

char *CLOX_STDCALL strnget(const char *const str, size_t count)
{
    char *result = stralloc(count);

    if (!str)
        return result;

    char *p = result;
    size_t i;

    for (i = 0; i < count; i++)
        *(p++) = str[i];

    return result;
}

char *CLOX_STDCALL strnmov(char *const dest, const char *const src, size_t count)
{
    if (!src || !count)
        return NULL;

    char *result;

    if (!dest)
        result = strnget(src, count);
    else
        result = strncpy(dest, src, count);

    return result;
}

char *CLOX_STDCALL strnlow(char *const dest, const char *const src, size_t count)
{
    char *result = strnmov(dest, src, count);

    if (!result)
        return NULL;

    size_t i;

    for (i = 0; i < count; i++)
        result[i] = tolower(result[i]);

    return result;
}

char *CLOX_STDCALL strnupp(char *const dest, const char *const src, size_t count)
{
    char *result = strnmov(dest, src, count);

    if (!result)
        return NULL;

    size_t i;

    for (i = 0; i < count; i++)
        result[i] = toupper(result[i]);

    return result;
}

#if (!CLOX_PLATFORM_IS_MACOS && !CLOX_PLATFORM_IS_WINDOWS) || (CLOX_PLATFORM_IS_WINDOWS && !CLOX_C_EXTENSIONS)

int CLOX_STDCALL stricmp(const char *const str1, const char *const str2)
{
    CLOX_REGISTER const unsigned char *p1 = (const unsigned char *)str1;
    CLOX_REGISTER const unsigned char *p2 = (const unsigned char *)str2;

    unsigned char c1, c2;

    do
    {
        c1 = (unsigned char)*(p1++);
        c2 = (unsigned char)*(p2++);

        c1 = tolower(c1);
        c2 = tolower(c2);

        if (c1 == NUL)
            return c1 - c2;
        else
            continue;
    } while (c1 == c2);

    return c1 - c2;
}

#endif

bool_t CLOX_STDCALL streq(const char *const str1, const char *const str2)
{
    if (str1 == str2)
        return TRUE;

    if (!str1 || !str2 || (*str1 != *str2))
        return FALSE;

    size_t len = strlen(str1);

    if (len != strlen(str2))
        return FALSE;

    --len;

    for (; len > 0; len--)
    {
        if (str1[len] != str2[len])
            return FALSE;
        else
            continue;
    }

    return TRUE;
}

bool_t CLOX_STDCALL striq(const char *const str1, const char *const str2)
{
    if (str1 == str2)
        return TRUE;

    if (!str1 || !str2)
        return FALSE;

    if (tolower(*str1) != tolower(*str2))
        return FALSE;

    size_t len = strlen(str1);

    if (len != strlen(str2))
        return FALSE;

    --len;

    CLOX_REGISTER char c1, c2;

    for (; len > 0; len--)
    {
        c1 = str1[len];
        c2 = str2[len];

        if ((c1 != c2) && (tolower(c1) != tolower(c2)))
            return FALSE;
        else
            continue;
    }

    return TRUE;
}
