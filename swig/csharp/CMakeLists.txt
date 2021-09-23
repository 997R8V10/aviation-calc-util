option(BUILD_CSHARP "Build CSharp SWIG module" ON)

# InteropTools
set_property(SOURCE tools/boost_gregorian_date.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE tools/boost_gregorian_date.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.InteropTools)

# GeoTools
set_property(SOURCE GeoTools/GeoPoint.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/GeoPoint.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.GeoTools)
set_property(SOURCE GeoTools/GeoUtil.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/GeoUtil.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.GeoTools)
set_property(SOURCE GeoTools/AtmosUtil.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/AtmosUtil.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.GeoTools)
set_property(SOURCE GeoTools/GeoTile.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/GeoTile.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.GeoTools)

# GeoTools/MagneticTools
set_property(SOURCE GeoTools/MagneticTools/MagneticFieldElements.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/MagneticTools/MagneticFieldElements.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.GeoTools.MagneticTools)
set_property(SOURCE GeoTools/MagneticTools/MagneticModelCoefficients.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/MagneticTools/MagneticModelCoefficients.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.GeoTools.MagneticTools)


# Build Bridge Library
swig_add_library(aviationcalc_netbrg
        TYPE SHARED
        LANGUAGE csharp
        OUTPUT_DIR ${PROJECT_BINARY_DIR}/wrappers/dotnet
        SOURCES
        tools/boost_gregorian_date.i
        GeoTools/GeoPoint.i
        GeoTools/GeoUtil.i
        GeoTools/AtmosUtil.i
        GeoTools/GeoTile.i
        GeoTools/MagneticTools/MagneticFieldElements.i
        GeoTools/MagneticTools/MagneticModelCoefficients.i
        )

set_target_properties(aviationcalc_netbrg PROPERTIES
        SWIG_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/genheaders"
        SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
        POSITION_INDEPENDENT_CODE ON)
target_link_libraries(aviationcalc_netbrg PRIVATE aviationcalc)