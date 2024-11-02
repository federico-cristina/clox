define_property(GLOBAL PROPERTY CLOX_TARGETS)
define_property(GLOBAL PROPERTY CLOX_LIBRARY)

set(CLOX_LOG_PREFIX       "clox")
set(CLOX_LIBRARY_PREFIX   "clox-lib-")
set(CLOX_UNIT_TEST_PREFIX "clox-test-")

set(CMAKE_STATIC_LIBRARY_PREFIX ${CLOX_LIBRARY_PREFIX})
set(CMAKE_SHARED_LIBRARY_PREFIX ${CLOX_LIBRARY_PREFIX})
set(CMAKE_IMPORT_LIBRARY_PREFIX ${CLOX_LIBRARY_PREFIX})

set(CLOX_INCLUDE_PREFIX "${CLOX_INCLUDE_DIR}/clox")

include(clox_log)
include(clox_add_executable)
include(clox_add_library)
include(clox_add_unit_test)
include(clox_link_libraries)
