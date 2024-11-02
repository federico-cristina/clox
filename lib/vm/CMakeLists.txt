set(HEADERS
    "buffer.h"
)

set(SOURCES
    "buffer.c"
)

clox_add_library(vm
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS base
    INSTALL
)
