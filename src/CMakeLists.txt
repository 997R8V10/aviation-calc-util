# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.76.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
# End Boost

include(FetchContent)

# ecCodes
FetchContent_Declare(
        ecCodes
        URL https://confluence.ecmwf.int/download/attachments/45757960/eccodes-2.22.1-Source.tar.gz
)

FetchContent_GetProperties(ecCodes)
if (NOT eccodes_POPULATED)
    FetchContent_Populate(ecCodes)

    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/eccodes/install CACHE INTERNAL "")
    set(ENABLE_NETCDF OFF CACHE INTERNAL "")
    set(ENABLE_JPG OFF CACHE INTERNAL "")
    set(ENABLE_FORTRAN OFF CACHE INTERNAL "")
    set(ENABLE_PRODUCT_BUFR OFF CACHE INTERNAL "")
    set(ENABLE_BUILD_TOOLS OFF CACHE INTERNAL "")

    IF (WIN32)
        # Copy an additional/replacement file into the populated source
        file(COPY ecbuild-replace/ecbuild_check_os.cmake DESTINATION ${eccodes_SOURCE_DIR}/cmake)
    ENDIF ()

    add_subdirectory(${eccodes_SOURCE_DIR} ${eccodes_BINARY_DIR})
endif ()
# End ecCodes

add_library(aviationcalc SHARED
        GeoTools/GeoUtil.cpp
        GeoTools/GeoPoint.cpp
        GeoTools/GribTools/GribDataPoint.cpp
        GeoTools/GribTools/GribTile.cpp
        MathTools/MathUtil.cpp
        MathTools/LegendreUtil.cpp
        MathTools/Polynomial.cpp)

target_include_directories(aviationcalc PRIVATE
        ${eccodes_SOURCE_DIR}
        ${eccodes_BINARY_DIR}
        ${eccodes_SOURCE_DIR}/src
        ${eccodes_BINARY_DIR}/src
        )

target_include_directories(aviationcalc PUBLIC
        ${PROJECT_SOURCE_DIR}/include/GeoTools
        ${PROJECT_SOURCE_DIR}/include/GeoTools/GribTools
        ${PROJECT_SOURCE_DIR}/include/MathTools
        ${PROJECT_SOURCE_DIR}/include/Timers
        )

set_target_properties(aviationcalc
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

if (WIN32)
    target_link_libraries(aviationcalc Urlmon.lib ${BOOST_LIBRARIES} eccodes)
else ()
    target_link_libraries(aviationcalc ${BOOST_LIBRARIES} eccodes)
endif ()