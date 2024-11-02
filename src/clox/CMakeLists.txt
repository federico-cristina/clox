set(SOURCES
    "main.c"
)

clox_add_executable(clox
    SOURCES ${SOURCES}
    INSTALL
)
