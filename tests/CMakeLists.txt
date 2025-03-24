set(BYTE_ARRAY_TEST_TARGET byte_array_test)
set(FILE_IO_TEST_TARGET file_io_test)

add_executable(${BYTE_ARRAY_TEST_TARGET} byte_array_test.cpp)
target_link_libraries(${BYTE_ARRAY_TEST_TARGET}
    PRIVATE
    msh_utils
    Catch2::Catch2WithMain
)

add_executable(${FILE_IO_TEST_TARGET} file_io_test.cpp)
target_link_libraries(${FILE_IO_TEST_TARGET}
    PRIVATE
    msh_utils
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(${BYTE_ARRAY_TEST_TARGET})
catch_discover_tests(${FILE_IO_TEST_TARGET})

# Configure coverage if enabled
if(ENABLE_COVERAGE)
    configure_opencppcoverage(${BYTE_ARRAY_TEST_TARGET} "${CMAKE_SOURCE_DIR}/include/msh/utils/byte_array.hpp")
    configure_opencppcoverage(${FILE_IO_TEST_TARGET} "${CMAKE_SOURCE_DIR}/include/msh/utils/file_io.hpp")
endif()
