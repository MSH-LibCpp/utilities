cmake_minimum_required(VERSION 3.14)
project(msh_utils VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Code Coverage Configuration
option(ENABLE_COVERAGE "Enable coverage reporting" ON)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cppcheck)
include(opencppcoverage)

# Configure static analysis
configure_cppcheck()

# Create interface library for header-only implementation
add_library(msh_utils INTERFACE)
target_include_directories(msh_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Create a static library version for testing and coverage
add_library(msh_utils_static INTERFACE)
target_include_directories(msh_utils_static INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install targets
install(TARGETS msh_utils
    EXPORT msh_utils-targets
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# Export targets
install(EXPORT msh_utils-targets
    FILE msh_utils-targets.cmake
    NAMESPACE msh::
    DESTINATION lib/cmake/msh_utils
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
    INSTALL_DESTINATION lib/cmake/msh_utils
)

install(FILES
    "${CMAKE_BINARY_DIR}/msh_utils-config.cmake"
    "${CMAKE_BINARY_DIR}/msh_utils-config-version.cmake"
    DESTINATION lib/cmake/msh_utils
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