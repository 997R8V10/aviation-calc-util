cmake_minimum_required(VERSION 3.14)
project(aviation-calc-util
        VERSION 0.0.7
        DESCRIPTION "A library to assist with aviation related calculations."
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# Includes
include(FetchContent)
include(GenerateExportHeader)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
# End Conan

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.76.0 REQUIRED COMPONENTS filesystem)
# End Boost

# ecCodes
find_package(eccodes 2.22.1 REQUIRED)
# End ecCodes

add_subdirectory(src)

# Install
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)