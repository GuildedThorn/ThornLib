add_executable(thorn_example demo.c)

target_link_libraries(thorn_example PRIVATE Thorn::thorn)

thorn_set_warnings(thorn_example)
if(THORN_ENABLE_SANITIZERS)
    thorn_enable_sanitizers(thorn_example)
endif()
