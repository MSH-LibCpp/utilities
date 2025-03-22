add_executable(byte_array_test byte_array_test.cpp)
target_link_libraries(byte_array_test
    PRIVATE
        msh_utils_static
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(byte_array_test)

# Configure coverage if enabled
if(ENABLE_COVERAGE)
    configure_opencppcoverage(byte_array_test msh_utils_static)
endif() 