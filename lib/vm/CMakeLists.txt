set(HEADERS
    "chunk.h"
    "opcode.h"
)

set(SOURCES
    "chunk.c"
    "opcode.c"
)

clox_add_library(vm
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS base
    INSTALL
)
