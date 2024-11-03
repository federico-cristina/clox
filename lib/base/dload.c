/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 */

#include "clox/base/dload.h"

#if CLOX_PLATFORM_IS_WINDOWS
#   include <windows.h>
#else
#   include <dlfcn.h>
#endif

handle_t CLOX_STDCALL dlload_current(void)
{
    static handle_t handle = NULL;

    if (!handle)
#if CLOX_PLATFORM_IS_WINDOWS
        handle = (handle_t)GetModuleHandleA(NULL);
#else
        handle = (handle_t)dlopen(NULL, RTLD_LAZY);
#endif

    return handle;
}

handle_t CLOX_STDCALL dlload(const char *const path)
{
    handle_t handle;

#if CLOX_PLATFORM_IS_WINDOWS
    handle = (handle_t)LoadLibraryA((LPCSTR)path);
#else
    handle = (handle_t)dlopen(path, RTLD_LAZY);
#endif

    return handle;
}

handle_t CLOX_STDCALL dlimpf(handle_t handle, const char *const name)
{
    handle_t func;

#if CLOX_PLATFORM_IS_WINDOWS
    func = (handle_t)GetProcAddress((HMODULE)handle, (LPCSTR)name);
#else
    func = (handle_t)dlsym((void *)handle, name);
#endif

    return func;
}

bool_t CLOX_STDCALL dlunload(handle_t handle)
{
    bool_t result;

#if CLOX_PLATFORM_IS_WINDOWS
    result = (bool_t)FreeLibrary((HMODULE)handle);
#else
    result = (bool_t)!dlclose((void *)handle);
#endif

    return result;
}
