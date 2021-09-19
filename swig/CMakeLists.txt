find_package(SWIG)

if (${SWIG_FOUND})
    include(${SWIG_USE_FILE})

    # C-Sharp
    option(BUILD_CSHARP "Build CSharp SWIG module" ON)
    set_property(SOURCE GeoTools/GeoPoint.i PROPERTY CPLUSPLUS ON)
    set_property(SOURCE GeoTools/GeoPoint.i PROPERTY COMPILE_OPTIONS
            -namespace AviationCalcUtilNet.GeoTools)
    set_property(SOURCE GeoTools/GeoUtil.i PROPERTY CPLUSPLUS ON)
    set_property(SOURCE GeoTools/GeoUtil.i PROPERTY COMPILE_OPTIONS
            -namespace AviationCalcUtilNet.GeoTools)
    swig_add_library(aviationcalc_netbrg
            TYPE USE_BUILD_SHARED_LIBS
            LANGUAGE csharp
            OUTPUT_DIR ${PROJECT_BINARY_DIR}/dotnet
            SOURCES GeoTools/GeoPoint.i GeoTools/GeoUtil.i)

    set_target_properties(aviationcalc_netbrg PROPERTIES
            SWIG_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/genheaders"
            SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
            POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(aviationcalc_netbrg PRIVATE aviationcalc)
endif ()