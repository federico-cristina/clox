# Usage:
#
#   clox_add_executable(<TARGET> SOURCES <SOURCES> [HEADERS <HEADERS>] [DEPENDS <DEPENDENCIES>] [[INSTALL] [DESTINATION <PATH>]])
#
# Adds an executable target.
#
function(clox_add_executable TARGET)
    set(_OPTIONS INSTALL)
    set(_ONE_VAL DESTINATION)
    set(_MUL_VAL SOURCES HEADERS DEPENDS)

    cmake_parse_arguments(_ARG
        "${_OPTIONS}"
        "${_ONE_VAL}"
        "${_MUL_VAL}"
         ${ARGV}
    )

    set(_EXE_HEADERS "")

    if(_ARG_HEADERS)
        foreach(_ARG_HEADER ${_ARG_HEADERS})
            list(APPEND _EXE_HEADERS "${CLOX_INCLUDE_PREFIX}/${TARGET}/${_ARG_HEADER}")
        endforeach()
    endif()

    add_executable(${TARGET} "${_EXE_HEADERS};${_ARG_SOURCES}")

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

    clox_log("add executable ${TARGET}")
endfunction()
