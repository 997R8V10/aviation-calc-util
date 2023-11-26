cmake_minimum_required(VERSION 3.14)
project(aviation-calc-util
        VERSION 1.2.1
        DESCRIPTION "A library to assist with aviation related calculations."
        HOMEPAGE_URL "https://github.com/997R8V10/aviation-calc-util"
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmakescripts)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Includes
include(FetchContent)
include(GenerateExportHeader)

# Conan
# Download if conan.cmake is not found
#if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
#    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan_provider.cmake")
#        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
#        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/451fa97d2c59c07b13fb4812a64b2a6391f9e781/conan_provider.cmake"
#                "${CMAKE_BINARY_DIR}/conan_provider.cmake"
##                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
#                TLS_VERIFY ON)
#    endif ()
#
#    include(${CMAKE_BINARY_DIR}/conan_provider.cmake)
#
#    conan_cmake_autodetect(conan_settings)
#    conan_cmake_install(PATH_OR_REFERENCE "${CMAKE_SOURCE_DIR}/conanfile.py" SETTINGS ${conan_settings} BUILD missing)
#endif ()

#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup(KEEP_RPATHS)
#include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
# End Conan

# MacOS RPath
if (APPLE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH 1)
    set(CMAKE_INSTALL_RPATH "@loader_path")
elseif(UNIX)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH 1)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()
# End MacOS RPath

# Boost
set(Boost_NO_SYSTEM_PATHS ON)
if (BUILD_SHARED_LIBS)
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_STATIC_RUNTIME OFF)
else()
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_STATIC_RUNTIME ON)
endif()
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.76.0 REQUIRED)
# End Boost

# Filesystem
find_package(Filesystem REQUIRED COMPONENTS Experimental Final)
# End Filesystem

# ecCodes
find_package(eccodes 2.22.1 REQUIRED)
# End ecCodes

# Curl
if (NOT WIN32)
    find_package(CURL REQUIRED)
endif ()
# End Curl

add_subdirectory(src)

add_subdirectory(test_package)

# Install
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)