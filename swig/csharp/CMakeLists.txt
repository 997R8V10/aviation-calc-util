option(BUILD_CSHARP "Build CSharp SWIG module" ON)

# InteropTools
set_property(SOURCE tools/boost_gregorian_date.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE tools/boost_gregorian_date.i PROPERTY COMPILE_OPTIONS
        -namespace AviationCalcUtilNet.InteropTools)

# GeoTools
set_property(SOURCE GeoTools/GeoPoint.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/GeoUtil.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/AtmosUtil.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/GeoTile.i PROPERTY CPLUSPLUS ON)

# GeoTools/MagneticTools
set_property(SOURCE GeoTools/MagneticTools/MagneticFieldElements.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/MagneticTools/MagneticModelCoefficients.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/MagneticTools/MagneticModel.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/MagneticTools/MagneticResult.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/MagneticTools/MagneticTile.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/MagneticTools/MagneticUtil.i PROPERTY CPLUSPLUS ON)

# GeoTools/GribTools
set_property(SOURCE GeoTools/GribTools/GribDataPoint.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE GeoTools/GribTools/GribTile.i PROPERTY CPLUSPLUS ON)

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
        GeoTools/MagneticTools/MagneticModel.i
        GeoTools/MagneticTools/MagneticResult.i
        GeoTools/MagneticTools/MagneticTile.i
        GeoTools/MagneticTools/MagneticUtil.i
        GeoTools/GribTools/GribDataPoint.i
        GeoTools/GribTools/GribTile.i
        )

set_target_properties(aviationcalc_netbrg PROPERTIES
        SWIG_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/genheaders"
        SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
        POSITION_INDEPENDENT_CODE ON)
target_link_libraries(aviationcalc_netbrg PRIVATE aviationcalc)