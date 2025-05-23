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
set(MSH_UTILS_TARGET msh_utils)
add_library(${MSH_UTILS_TARGET} INTERFACE)
add_library(msh::utils ALIAS ${MSH_UTILS_TARGET})
target_link_libraries(${MSH_UTILS_TARGET} INTERFACE plog::plog nlohmann_json::nlohmann_json)

# Set export name
# find_package(msh_utils REQUIRED)
# target_link_libraries(myapp PRIVATE msh::utils)
set_target_properties(${MSH_UTILS_TARGET} PROPERTIES EXPORT_NAME utils)

# Set include directories
target_include_directories(${MSH_UTILS_TARGET} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include($ENV{MSH_ROOT_PATH}/configs/cmake/cppcheck.cmake)
if(ENABLE_STATIC_ANALYSIS)
    configure_cppcheck(
        TARGET ${MSH_UTILS_TARGET}
        PATHS
            "${CMAKE_CURRENT_SOURCE_DIR}/include/msh/utils"
    )
endif()

include($ENV{MSH_ROOT_PATH}/configs/cmake/installpackages.cmake)
install_interface_library(
    TARGET ${MSH_UTILS_TARGET}
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
