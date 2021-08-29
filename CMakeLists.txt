cmake_minimum_required(VERSION 3.5)
project(aviation_calc_util)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# Includes
include(FetchContent)
include(GenerateExportHeader)

# Conan
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
            TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conan-center
        SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
# End Conan

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.76.0 REQUIRED COMPONENTS filesystem)
# End Boost

# ecCodes
FetchContent_Declare(
        ecCodes
        URL https://confluence.ecmwf.int/download/attachments/45757960/eccodes-2.22.1-Source.tar.gz
)

FetchContent_GetProperties(ecCodes)
if (NOT eccodes_POPULATED)
    FetchContent_Populate(ecCodes)

    set(ENABLE_NETCDF OFF CACHE INTERNAL "")
    set(ENABLE_JPG OFF CACHE INTERNAL "")
    set(ENABLE_FORTRAN OFF CACHE INTERNAL "")
    set(ENABLE_PRODUCT_BUFR OFF CACHE INTERNAL "")
    set(ENABLE_BUILD_TOOLS OFF CACHE INTERNAL "")
    set(ENABLE_EXAMPLES OFF CACHE INTERNAL "")
    set(ENABLE_TESTS OFF CACHE INTERNAL "")
    set(ENABLE_INSTALL_ECCODES_DEFINITIONS ON CACHE INTERNAL "")
    set(ENABLE_INSTALL_ECCODES_SAMPLES OFF CACHE INTERNAL "")

    IF (WIN32)
        set(ENABLE_ECCODES_THREADS OFF CACHE INTERNAL "")
        set(ENABLE_ECCODES_OMP_THREADS ON CACHE INTERNAL "")

        # Copy an additional/replacement file into the populated source
        file(COPY ${PROJECT_SOURCE_DIR}/cmakescripts/ecbuild_check_os.cmake DESTINATION ${eccodes_SOURCE_DIR}/cmake)
    ELSE ()
        set(ENABLE_ECCODES_THREADS ON CACHE INTERNAL "")
        set(ENABLE_ECCODES_OMP_THREADS OFF CACHE INTERNAL "")
    ENDIF ()

    add_subdirectory(${eccodes_SOURCE_DIR} ${eccodes_BINARY_DIR})

    include_directories(
            ${eccodes_SOURCE_DIR}
            ${eccodes_BINARY_DIR}
            ${eccodes_SOURCE_DIR}/src
            ${eccodes_BINARY_DIR}/src
    )
endif ()
# End ecCodes

add_subdirectory(src)
add_subdirectory(tests)