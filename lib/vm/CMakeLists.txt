set(HEADERS
    "chunk.h"
    "debug.h"
    "opcode.h"
)

set(SOURCES
    "chunk.c"
    "debug.c"
    "opcode.c"
)

clox_add_library(vm
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS base
    INSTALL
)
