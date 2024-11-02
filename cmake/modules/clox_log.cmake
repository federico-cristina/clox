if(NOT CLOX_LOG_PREFIX)
    set(CLOX_LOG_PREFIX "clox")
endif()

# Usage:
#
#   clox_log(<MESSAGE>)
#
# Notices a message with the CLOX_LOG_PREFIX prefix.
#
function(clox_log MESSAGE)
    message(NOTICE "${CLOX_LOG_PREFIX}: ${MESSAGE}")
endfunction()
