### Aviation Calc Util CMake File


# Includes
include(FetchContent)
include(GenerateExportHeader)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.76.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
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
    set(ENABLE_INSTALL_ECCODES_DEFINITIONS OFF CACHE INTERNAL "")
    set(ENABLE_INSTALL_ECCODES_SAMPLES OFF CACHE INTERNAL "")

    IF (WIN32)
        # Copy an additional/replacement file into the populated source
        file(COPY ecbuild-replace/ecbuild_check_os.cmake DESTINATION ${eccodes_SOURCE_DIR}/cmake)
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

# Add Source Files
add_library(aviationcalc SHARED
        GeoTools/GeoUtil.cpp
        GeoTools/GeoPoint.cpp
        GeoTools/GribTools/GribDataPoint.cpp
        GeoTools/GribTools/GribTile.cpp
        GeoTools/MagneticTools/MagneticUtil.cpp
        GeoTools/MagneticTools/MagneticFieldElements.cpp
        GeoTools/MagneticTools/MagneticResult.cpp
        GeoTools/MagneticTools/MagneticModelCoefficients.cpp
        GeoTools/MagneticTools/MagneticModel.cpp
        MathTools/MathUtil.cpp
        MathTools/LegendreUtil.cpp
        MathTools/Polynomial.cpp
        )

# Set Include Folders
target_include_directories(aviationcalc PUBLIC
        ${CMAKE_BINARY_DIR}/exports
        ${PROJECT_SOURCE_DIR}/include/GeoTools
        ${PROJECT_SOURCE_DIR}/include/GeoTools/MagneticTools
        ${PROJECT_SOURCE_DIR}/include/GeoTools/GribTools
        ${PROJECT_SOURCE_DIR}/include/MathTools
        ${PROJECT_SOURCE_DIR}/include/Timers
        )

# Set Output Folders
set_target_properties(aviationcalc
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        )

# Link Libraries
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    target_link_libraries(aviationcalc Urlmon.lib ${BOOST_LIBRARIES} eccodes)
else ()
    target_link_libraries(aviationcalc ${BOOST_LIBRARIES} eccodes curl)
endif ()

# DLL Export Headers
GENERATE_EXPORT_HEADER(aviationcalc
        BASE_NAME AVIATIONCALC
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/aviationcalc_exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)