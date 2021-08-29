### Aviation Calc Util CMake File

# Get Headers
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

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
        ${PROJECT_SOURCE_DIR}/include
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
    target_link_libraries(aviationcalc
            Urlmon
            Boost::boost
            Boost::filesystem
            Boost::disable_autolinking
            Boost::dynamic_linking
            eccodes)
else ()
    find_package(CURL REQUIRED)
    target_link_libraries(aviationcalc Boost::boost Boost::filesystem eccodes CURL::CURL)
endif ()

# DLL Export Headers
GENERATE_EXPORT_HEADER(aviationcalc
        BASE_NAME AVIATIONCALC
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/aviationcalc_exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

# ECCodes Definitions Files
add_custom_command(TARGET aviationcalc
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -Dsrc_dir="${eccodes_SOURCE_DIR}/definitions"
        -Ddst_dir="${CMAKE_BINARY_DIR}/bin/eccodes/definitions"
        -P "${PROJECT_SOURCE_DIR}/cmakescripts/update_eccodes_definitions.cmake"
        )