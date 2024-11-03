set(HEADERS
    "source_buffer.h"
    "source_location.h"
)

set(SOURCES
    "source_buffer.c"
)

clox_add_library(source
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS base
    INSTALL
)
