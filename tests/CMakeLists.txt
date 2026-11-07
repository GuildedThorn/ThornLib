add_executable(thorn_test_version core/test_version.c)

target_link_libraries(thorn_test_version PRIVATE Thorn::thorn)
target_include_directories(thorn_test_version PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

thorn_set_warnings(thorn_test_version)
if(THORN_ENABLE_SANITIZERS)
    thorn_enable_sanitizers(thorn_test_version)
endif()

add_test(NAME core/version COMMAND thorn_test_version)
