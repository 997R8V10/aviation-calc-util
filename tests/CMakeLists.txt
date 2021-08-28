ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test_geopoint test_geopoint.cpp)

target_link_libraries(test_geopoint aviationcalc)

add_custom_command(TARGET test_geopoint POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/bin"
        "${CMAKE_CURRENT_BINARY_DIR}"
        )