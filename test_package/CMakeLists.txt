#cmake_minimum_required(VERSION 3.14)
#project(PackageTest CXX)
#
#set(CMAKE_CXX_STANDARD 20)
#
##include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
##conan_basic_setup(KEEP_RPATHS)
#
## MacOS RPath
#if (APPLE)
#    set(CMAKE_BUILD_WITH_INSTALL_RPATH 1)
#    set(CMAKE_INSTALL_RPATH "@loader_path")
#endif()
#
## End MacOS RPath
#find_package(aviationcalc CONFIG REQUIRED)
#
#add_executable(test_geopoint test_geopoint.cpp)
#target_link_libraries(test_geopoint aviationcalc::aviationcalc)
#
#add_executable(test_atmos test_atmos.cpp)
#target_link_libraries(test_atmos aviationcalc::aviationcalc)
#
## CTest is a testing tool that can be used to test your project.
#enable_testing()
#add_test(NAME test_atmos
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#         COMMAND test_atmos)
#
#
#add_test(NAME test_geopoint
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#        COMMAND test_geopoint)


### Test Executables

add_executable(test_atmos test_atmos.cpp)
target_link_libraries(test_atmos aviationcalc)

set_target_properties(test_atmos
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        )

add_executable(test_geopoint test_geopoint.cpp)
target_link_libraries(test_geopoint aviationcalc)

set_target_properties(test_geopoint
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        )


add_executable(test_geoutil test_geoutil.cpp)
target_link_libraries(test_geoutil aviationcalc)

set_target_properties(test_geoutil
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        )