# Usage:
#
#   clox_add_library(<TARGET> SOURCES <SOURCES> [HEADERS <HEADERS>] [STATIC|SHARED] [DEPENDS <DEPENDENCIES>] [[INSTALL] [DESTINATION <PATH>]])
#
# Adds a library target with clox specific naming and appends it into the global
# properties CLOX_LIBRARY and CLOX_TARGETS.
#
function(clox_add_library TARGET)
    set(_OPTIONS STATIC SHARED INSTALL)
    set(_ONE_VAL DESTINATION)
    set(_MUL_VAL SOURCES HEADERS DEPENDS)

    cmake_parse_arguments(_ARG
        "${_OPTIONS}"
        "${_ONE_VAL}"
        "${_MUL_VAL}"
         ${ARGV}
    )

    set(_LIB_NAME "${CLOX_LIBRARY_PREFIX}${TARGET}")

    if(_ARG_HEADERS AND NOT _ARG_SOURCES)
        set(_LIB_TYPE INTERFACE)
    elseif(_ARG_STATIC AND _ARG_SHARED)
        set(_LIB_TYPE STATIC SHARED)
    elseif(_ARG_STATIC)
        set(_LIB_TYPE STATIC)
    elseif(_ARG_SHARED)
        set(_LIB_TYPE SHARED)
    else()
        set(_LIB_TYPE ${CLOX_DEFAULT_LIB_TYPE})
    endif()

    set(_LIB_HEADERS "")

    if(_ARG_HEADERS)
        foreach(_ARG_HEADER ${_ARG_HEADERS})
            list(APPEND _LIB_HEADERS "${CLOX_INCLUDE_PREFIX}/${TARGET}/${_ARG_HEADER}")
        endforeach()
    endif()

    add_library(${TARGET} ${_LIB_TYPE} "${_LIB_HEADERS};${_ARG_SOURCES}")

    if(_ARG_DEPENDS)
        target_link_libraries(${TARGET} ${_ARG_DEPENDS})
    endif()

    if(_ARG_INSTALL)
        if(_ARG_DESTINATION)
            install(TARGETS ${TARGET} ${_ARG_DEPENDS} DESTINATION ${_ARG_DESTINATION})
        else()
            install(TARGETS ${TARGET} ${_ARG_DEPENDS})
        endif()
    endif()

    set_property(GLOBAL APPEND PROPERTY CLOX_TARGETS "${TARGET}")
    set_property(GLOBAL APPEND PROPERTY CLOX_LIBRARY "${_LIB_NAME}")

    clox_log("add library ${_LIB_NAME}")
endfunction()
