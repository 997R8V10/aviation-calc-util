cmake_minimum_required(VERSION 3.14)
project(aviation-calc-util
        VERSION 0.0.1
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
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/eccodes)

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

# Install
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
install(TARGETS aviationcalc
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY "${CMAKE_BINARY_DIR}/exports/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILES_MATCHING
        PATTERN "*.dll"
        PATTERN "*.pdb"
        PATTERN "*.ilk"
        )