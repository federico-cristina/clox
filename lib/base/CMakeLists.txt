set(HEADERS
    "defs.h"
    "bool.h"
    "utils.h"
    "byte.h"
    "api.h"
    "errno.h"
    "bits.h"
    "file.h"
    "string.h"
    "utf8.h"
)

set(SOURCES
    "errno.c"
    "string.c"
    "utf8.c"
)

clox_add_library(base
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    INSTALL
)
