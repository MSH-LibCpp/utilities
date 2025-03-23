add_executable(byte_array_test byte_array_test.cpp)
target_link_libraries(byte_array_test
    PRIVATE
    msh_utils
    Catch2::Catch2WithMain
)

add_executable(file_io_test file_io_test.cpp)
target_link_libraries(file_io_test
    PRIVATE
    msh_utils
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(byte_array_test)
catch_discover_tests(file_io_test)

# Configure coverage if enabled
if(ENABLE_COVERAGE)
    configure_opencppcoverage(byte_array_test "${CMAKE_SOURCE_DIR}/include/msh/utils/byte_array.hpp")
    configure_opencppcoverage(file_io_test "${CMAKE_SOURCE_DIR}/include/msh/utils/file_io.hpp")
endif()
