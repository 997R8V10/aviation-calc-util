cmake_minimum_required(VERSION 3.14)
project(aviation-calc-util
        VERSION 0.3.0
        DESCRIPTION "A library to assist with aviation related calculations."
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# Includes
include(FetchContent)
include(GenerateExportHeader)

# Conan
# Download if conan.cmake is not found
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" OR NOT EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_autodetect(conan_settings)
    conan_cmake_install(PATH_OR_REFERENCE "${CMAKE_SOURCE_DIR}/conanfile.py" SETTINGS ${conan_settings} BUILD missing)
endif ()

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

# Curl
if (NOT WIN32)
    find_package(CURL REQUIRED)
endif ()
# End Curl

add_subdirectory(src)

add_executable(test_atmos test_package/test_atmos.cpp)
target_link_libraries(test_atmos aviationcalc)

# Install
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)