# AddressSanitizer + UndefinedBehaviorSanitizer support.
#
# Usage:
#   thorn_enable_sanitizers(<target>)
#
# Call this for every executable and library target; sanitizer link flags do
# not propagate through static libraries.

include_guard(GLOBAL)

function(thorn_enable_sanitizers target)
    if(MSVC)
        message(WARNING "Sanitizers are not configured for MSVC; skipping ${target}")
        return()
    endif()

    target_compile_options(${target} PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -fno-sanitize-recover=all
    )

    target_link_options(${target} PRIVATE
        -fsanitize=address,undefined
    )
endfunction()
