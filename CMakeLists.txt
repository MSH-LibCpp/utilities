cmake_minimum_required(VERSION 3.20)

set(PROJECT_VERSION "1.1.0")

project(
    msh_utils
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX
)

# C++ Standard Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project Options
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
option(ENABLE_STATIC_ANALYSIS "Enable static analysis" ON)
option(BUILD_TESTING "Build test suite" ON)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cppcheck)
include(opencppcoverage)

find_package(Plog REQUIRED)

# Create interface library for header-only implementation
add_library(msh_utils INTERFACE)
add_library(msh::utils ALIAS msh_utils)
target_link_libraries(msh_utils INTERFACE plog::plog)

# Set include directories
target_include_directories(msh_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set(MSH_UTILS_STATIC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/byte_array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_io.cpp
)

# Create a static library version for testing and coverage
add_library(msh_utils_static STATIC ${MSH_UTILS_STATIC_SOURCES})
target_include_directories(msh_utils_static PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msh/utils
)
target_link_libraries(msh_utils_static PRIVATE msh_utils)

if(ENABLE_STATIC_ANALYSIS)
    configure_cppcheck(msh_utils_static "${CMAKE_CURRENT_SOURCE_DIR}/include/msh/utils")
endif()

# Install targets
install(TARGETS msh_utils
    EXPORT msh_utils-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

# Export targets
install(EXPORT msh_utils-targets
    FILE msh_utils-targets.cmake
    NAMESPACE msh::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/msh_utils
)

# Create and install package config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "msh_utils-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/msh_utils-config.cmake.in"
    "${CMAKE_BINARY_DIR}/msh_utils-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/msh_utils
)

install(FILES
    "${CMAKE_BINARY_DIR}/msh_utils-config.cmake"
    "${CMAKE_BINARY_DIR}/msh_utils-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/msh_utils
)

# Testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        # Find Catch2
        find_package(Catch2 3.5.3 REQUIRED)

        add_subdirectory(tests)
    endif()
endif()
