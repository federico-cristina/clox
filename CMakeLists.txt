cmake_minimum_required(VERSION 3.10.0)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project("clox"
        VERSION   0.1.0
        LANGUAGES C
    )

    set(CMAKE_C_EXTENSIONS        ON)
    set(CMAKE_C_STANDARD          99)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

option(CLOX_ENABLE_UNIT_TESTS "Enables unit tests targets." ON)

set(CLOX_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(CLOX_MODULES_DIR "${CLOX_DIR}/cmake/modules")
set(CLOX_INCLUDE_DIR "${CLOX_DIR}/include")
set(CLOX_LIBRARY_DIR "${CLOX_DIR}/lib")
set(CLOX_CONFIG_PATH "${CLOX_INCLUDE_DIR}/clox/config.h")

list(APPEND CMAKE_MODULE_PATH "${CLOX_MODULES_DIR}")

include(clox)

configure_file("${CLOX_CONFIG_PATH}.in" "${CLOX_CONFIG_PATH}")
clox_log("config file ${CLOX_CONFIG_PATH}")

include_directories("${CLOX_INCLUDE_DIR}")

add_subdirectory(lib)
add_subdirectory(src)

if(CLOX_ENABLE_UNIT_TESTS)
    add_subdirectory(units)
endif()
