### Aviation Calc Util CMake File

# Get Headers
set(HEADER_FILES
        ${PROJECT_SOURCE_DIR}/include/AviationCalc.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/GeoPoint.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/GeoUtil.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/GribTools/GribDataPoint.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/GribTools/GribTile.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/MagneticTools/MagneticFieldElements.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/MagneticTools/MagneticModelCoefficients.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/MagneticTools/MagneticModel.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/MagneticTools/MagneticResult.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/MagneticTools/MagneticUtil.h
        ${PROJECT_SOURCE_DIR}/include/MathTools/Polynomial.h
        ${PROJECT_SOURCE_DIR}/include/MathTools/LegendreUtil.h
        ${PROJECT_SOURCE_DIR}/include/MathTools/MathUtil.h
        )

# Add Source Files
add_library(aviationcalc SHARED
        ${HEADER_FILES}
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
            eccodes::eccodes)
else ()
    find_package(CURL REQUIRED)
    target_link_libraries(aviationcalc Boost::boost Boost::filesystem eccodes::eccodes CURL::CURL)
endif ()

# DLL Export Headers
GENERATE_EXPORT_HEADER(aviationcalc
        BASE_NAME AVIATIONCALC
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/aviationcalc_exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

# NuGet Package
if (WIN32)
    # Install Settings
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bits
        set(NUGET_ARCH x64)
    else()
        # 32 bits
        set(NUGET_ARCH x86)
    endif()

    set(NUGET_PACKAGE_NAME "PShivaraman.AviationCalcUtil")
	set(NUGET_BUILD_FOLDER "${CMAKE_SOURCE_DIR}/out/nuget")

    # Generate .nuspec file
    configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/template.nuspec
            ${CMAKE_BINARY_DIR}/nuget/${NUGET_PACKAGE_NAME}.nuspec
            )

	# Generate .props file
    configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/template.props
            ${CMAKE_BINARY_DIR}/nuget/build/native/${NUGET_PACKAGE_NAME}.props
            )
			
	# Generate .props file
    configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/pack-nuget.bat
            ${CMAKE_BINARY_DIR}/nuget/pack-nuget.bat
            )

	# Copy nuget files to output folder
    add_custom_command(TARGET aviationcalc
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -Dsrc_dir="${CMAKE_BINARY_DIR}/nuget"
            -Ddst_dir="${NUGET_BUILD_FOLDER}"
            -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_dir_update.cmake"
            )

    # Runtimes
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/out/nuget/runtimes/native/${NUGET_ARCH}/${CMAKE_BUILD_TYPE})
    add_custom_command(TARGET aviationcalc
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -Dsrc_dir="${CMAKE_BINARY_DIR}/bin"
            -Ddst_dir="${NUGET_BUILD_FOLDER}/runtimes/native/${NUGET_ARCH}/${CMAKE_BUILD_TYPE}"
            -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_bins.cmake"
            )

    # Libraries
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/out/nuget/lib/native/${NUGET_ARCH}/${CMAKE_BUILD_TYPE})
    add_custom_command(TARGET aviationcalc
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -Dsrc_dir="${CMAKE_BINARY_DIR}/lib"
            -Ddst_dir="${NUGET_BUILD_FOLDER}/lib/native/${NUGET_ARCH}/${CMAKE_BUILD_TYPE}"
            -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_dir_update.cmake"
            )

    # Includes
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/out/nuget/build/native/include)
    add_custom_command(TARGET aviationcalc
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -Dsrc_dir="${CMAKE_SOURCE_DIR}/include"
            -Ddst_dir="${NUGET_BUILD_FOLDER}/build/native/include"
            -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_dir_update.cmake"
            )

    add_custom_command(TARGET aviationcalc
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -Dsrc_dir="${CMAKE_BINARY_DIR}/exports"
            -Ddst_dir="${NUGET_BUILD_FOLDER}/build/native/include"
            -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_dir_update.cmake"
            )


    # ECCodes Definitions
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/out/nuget/build/native/eccodes)
    add_custom_command(TARGET aviationcalc
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -Dsrc_dir="${CMAKE_BINARY_DIR}/bin/eccodes/definitions"
            -Ddst_dir="${NUGET_BUILD_FOLDER}/build/native/eccodes/definitions"
            -P "${PROJECT_SOURCE_DIR}/cmakescripts/update_eccodes_definitions.cmake"
            )
endif ()