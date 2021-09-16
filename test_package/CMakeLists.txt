cmake_minimum_required(VERSION 3.14)
project(PackageTest CXX)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

# MacOS RPath
if (APPLE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH 1)
    set(CMAKE_INSTALL_RPATH "@loader_path")
endif()

# End MacOS RPath

add_executable(test_geopoint test_geopoint.cpp)
target_link_libraries(test_geopoint ${CONAN_LIBS})

add_executable(test_atmos test_atmos.cpp)
target_link_libraries(test_atmos ${CONAN_LIBS})

# CTest is a testing tool that can be used to test your project.
enable_testing()
add_test(NAME test_atmos
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND test_atmos)


add_test(NAME test_geopoint
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND test_geopoint)
