get_property(CLOXLIB GLOBAL PROPERTY CLOX_TARGETS)

set(SOURCES
    "main.c"
)

clox_add_executable(clox
    SOURCES ${SOURCES}
    DEPENDS ${CLOXLIB}
    INSTALL
)
