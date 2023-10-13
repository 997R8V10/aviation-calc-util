### Aviation Calc Util CMake File

# Get Headers
set(HEADER_FILES
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
        ${PROJECT_SOURCE_DIR}/include/GeoTools/GeoTile.h
        ${PROJECT_SOURCE_DIR}/include/GeoTools/AtmosUtil.h
        ${PROJECT_SOURCE_DIR}/include/InteropTools/InteropUtil.h
        ${CMAKE_BINARY_DIR}/genheaders/aviationcalc_exports.h
        ${CMAKE_BINARY_DIR}/genheaders/FileSystem.h)

# Add Source Files
add_library(aviationcalc
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
        GeoTools/GeoTile.cpp
        GeoTools/MagneticTools/MagneticTile.cpp
        GeoTools/AtmosUtil.cpp
        InteropTools/InteropUtil.cpp)

# Generate FileSystem.h file
configure_file(${PROJECT_SOURCE_DIR}/cmakescripts/FileSystem.h.in
        ${CMAKE_BINARY_DIR}/genheaders/FileSystem.h
        )

# Set Include Folders
target_include_directories(aviationcalc PUBLIC
        ${CMAKE_BINARY_DIR}/genheaders
        ${PROJECT_SOURCE_DIR}/include
        )

# Set Output Folders
set_target_properties(aviationcalc
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
        )

# Link Libraries
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    target_link_libraries(aviationcalc
            Urlmon
            Boost::boost
            eccodes::eccodes
            std::filesystem)
	
	# Tell Boost to NOT auto link against compiler specific libs
	add_definitions( -DBOOST_ALL_NO_LIB )
	add_definitions( -DBOOST_LIB_DIAGNOSTIC )
else ()
    target_link_libraries(aviationcalc Boost::boost eccodes::eccodes CURL::libcurl std::filesystem)
endif ()

# DLL Export Headers
GENERATE_EXPORT_HEADER(aviationcalc
        BASE_NAME AVIATIONCALC
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/genheaders/aviationcalc_exports.h)

# NuGet Package
set(NUGET_PACKAGE_NAME "PShivaraman.AviationCalcUtil")
set(NUGET_BUILD_FOLDER "${CMAKE_SOURCE_DIR}/out/nuget")
set(CREATE_NUGET_PACKAGE 0 CACHE STRING "Should NuGet Package Files be outputted")
set(COPY_NUGET_DEFS 0 CACHE STRING "Should NuGet Definition files be copied")

# Nuget RID Settings
SET(NUGET_ARCH "win-x64" CACHE STRING "Nuget Package Runtime ID")

if (CREATE_NUGET_PACKAGE)
    if (COPY_NUGET_DEFS)
        # Generate .nuspec file
        configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/template.nuspec
                ${CMAKE_BINARY_DIR}/nuget/${NUGET_PACKAGE_NAME}.nuspec
                )

        # Generate .targets file
        configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/native.targets
                ${CMAKE_BINARY_DIR}/nuget/build/${NUGET_PACKAGE_NAME}.targets
                )

#        configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/netstandard.targets
#                ${CMAKE_BINARY_DIR}/nuget/build/netstandard2.0/${NUGET_PACKAGE_NAME}.targets
#                )

        # Generate Docs
        configure_file(${PROJECT_SOURCE_DIR}/README.md
                ${CMAKE_BINARY_DIR}/nuget/README.md
                )

        configure_file(${PROJECT_SOURCE_DIR}/LICENSE
                ${CMAKE_BINARY_DIR}/nuget/LICENSE.txt
                )

        # Generate pack-nuget.bat
        if (WIN32)
            configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/pack-nuget.bat
                    ${CMAKE_BINARY_DIR}/nuget/pack-nuget.bat
                    )
        else ()
            # Generate pack-nuget.sh
            configure_file(${PROJECT_SOURCE_DIR}/packages/nuget/pack-nuget.sh
                    ${CMAKE_BINARY_DIR}/nuget/pack-nuget.sh
                    )
        endif ()

        # Copy nuget files to output folder
        add_custom_command(TARGET aviationcalc
                POST_BUILD
                COMMAND ${CMAKE_COMMAND}
                -Dsrc_dir="${CMAKE_BINARY_DIR}/nuget"
                -Ddst_dir="${NUGET_BUILD_FOLDER}"
                -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_dir_update.cmake"
                )
    endif ()

    # Runtimes
    file(MAKE_DIRECTORY ${NUGET_BUILD_FOLDER}/runtimes/${NUGET_ARCH})
    if (WIN32)
#        file(MAKE_DIRECTORY ${NUGET_BUILD_FOLDER}/lib/netstandard2.0)
#        file(MAKE_DIRECTORY ${NUGET_BUILD_FOLDER}/lib/native)
#        add_custom_command(TARGET aviationcalc
#                POST_BUILD
#                COMMAND ${CMAKE_COMMAND}
#                -Dsrc_dir="${CMAKE_BINARY_DIR}/bin"
#                -Ddst_dir="${NUGET_BUILD_FOLDER}/${NUGET_ARCH}/lib/native"
#                -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_bins.cmake"
#                )
#        add_custom_command(TARGET aviationcalc
#                POST_BUILD
#                COMMAND ${CMAKE_COMMAND}
#                -Dsrc_dir="${CMAKE_BINARY_DIR}/bin"
#                -Ddst_dir="${NUGET_BUILD_FOLDER}/${NUGET_ARCH}/lib/netstandard2.0"
#                -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_bins.cmake"
#                )
        add_custom_command(TARGET aviationcalc
                POST_BUILD
                COMMAND ${CMAKE_COMMAND}
                -Dsrc_dir="${CMAKE_BINARY_DIR}/bin"
                -Ddst_dir="${NUGET_BUILD_FOLDER}/runtimes/${NUGET_ARCH}"
                -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_bins.cmake"
                )
    else ()
        # Libraries
        add_custom_command(TARGET aviationcalc
                POST_BUILD
                COMMAND ${CMAKE_COMMAND}
                -Dsrc_dir="${CMAKE_BINARY_DIR}/lib"
                -Ddst_dir="${NUGET_BUILD_FOLDER}/runtimes/${NUGET_ARCH}"
                -P "${PROJECT_SOURCE_DIR}/cmakescripts/copy_dir_update.cmake"
                )
    endif ()
endif ()
