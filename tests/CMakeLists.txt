set(BYTE_ARRAY_TEST_TARGET byte_array_test)
set(FILE_IO_TEST_TARGET file_io_test)
set(JSON_CONFIG_TEST_TARGET json_config_test)

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

add_executable(${JSON_CONFIG_TEST_TARGET} json_config_test.cpp)
target_link_libraries(${JSON_CONFIG_TEST_TARGET}
    PRIVATE
    msh_utils
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(${BYTE_ARRAY_TEST_TARGET})
catch_discover_tests(${FILE_IO_TEST_TARGET})
catch_discover_tests(${JSON_CONFIG_TEST_TARGET})

# Configure coverage if enabled
if(ENABLE_COVERAGE AND WIN32)
    include($ENV{MSH_ROOT_PATH}/configs/cmake/opencppcoverage.cmake)

    configure_opencppcoverage(
        TARGET ${BYTE_ARRAY_TEST_TARGET}
        SOURCES "${CMAKE_SOURCE_DIR}/include/msh/utils"
    )
    configure_opencppcoverage(
        TARGET ${FILE_IO_TEST_TARGET}
        SOURCES "${CMAKE_SOURCE_DIR}/include/msh/utils"
    )
    configure_opencppcoverage(
        TARGET ${JSON_CONFIG_TEST_TARGET}
        SOURCES "${CMAKE_SOURCE_DIR}/include/msh/utils"
    )
endif()
