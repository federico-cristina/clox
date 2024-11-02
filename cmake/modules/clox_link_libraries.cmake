# Usage:
#
#   clox_link_libraries(<TARGET>)
#
# Links clox libraries to the specified target.
#
function(clox_link_libraries TARGET)
    get_property(_LIBS GLOBAL PROPERTY CLOX_TARGETS)
    target_link_libraries(${TARGET} "${_LIBS}")
    clox_log("linked libraries to ${TARGET}")
endfunction()
