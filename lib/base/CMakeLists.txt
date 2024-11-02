set(HEADERS
    "defs.h"
    "bool.h"
    "utils.h"
    "byte.h"
    "api.h"
    "errno.h"
    "bits.h"
    "file.h"
)

set(SOURCES
    "errno.c"
)

clox_add_library(base
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    INSTALL
)
