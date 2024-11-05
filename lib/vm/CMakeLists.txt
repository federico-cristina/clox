set(HEADERS
    "buffer.h"
    "opcode.h"
)

set(SOURCES
    "buffer.c"
    "opcode.c"
)

clox_add_library(vm
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS base
    INSTALL
)
