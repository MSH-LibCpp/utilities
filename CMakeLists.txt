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
option(BUILD_TESTS "Build test suite" ON)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(plog REQUIRED)
find_package(nlohmann_json REQUIRED)

# Create interface library for header-only implementation
add_library(msh_utils INTERFACE)
add_library(msh::utils ALIAS msh_utils)
target_link_libraries(msh_utils INTERFACE plog::plog nlohmann_json::nlohmann_json)

# Set include directories
target_include_directories(msh_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include($ENV{MSH_ROOT_PATH}/configs/cmake/cppcheck.cmake)
if(ENABLE_STATIC_ANALYSIS)
    configure_cppcheck(
        TARGET msh_utils
        PATHS
            "${CMAKE_CURRENT_SOURCE_DIR}/include/msh/utils"
    )
endif()

include($ENV{MSH_ROOT_PATH}/configs/cmake/installpackages.cmake)
install_interface_library(
    TARGET msh_utils
)

# Testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(BUILD_TESTS)
        # Find Catch2
        find_package(Catch2 3.5.3 REQUIRED)
        include(CTest)
        enable_testing()

        add_subdirectory(tests)
    endif()
endif()
