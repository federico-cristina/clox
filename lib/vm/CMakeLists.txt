set(HEADERS
    "code_block.h"
    "debug.h"
    "opcode.h"
    "value.h"
)

set(SOURCES
    "code_block.c"
    "debug.c"
    "opcode.c"
    "value.c"
)

clox_add_library(vm
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS base
    INSTALL
)
