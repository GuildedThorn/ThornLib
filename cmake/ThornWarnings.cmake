# Reusable strict warning set for C targets.
#
# Usage:
#   thorn_set_warnings(<target>)

include_guard(GLOBAL)

function(thorn_set_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        if(THORN_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
        return()
    endif()

    target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wcast-align
        -Wcast-qual
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wold-style-definition
        -Wwrite-strings
        -Wundef
        -Wvla
        -Wdouble-promotion
        -Wformat=2
        -Wswitch-enum
        -Wimplicit-fallthrough
        -fno-common
    )

    if(THORN_WARNINGS_AS_ERRORS)
        target_compile_options(${target} PRIVATE -Werror)
    endif()
endfunction()
