# Boost
find_package(Boost REQUIRED COMPONENTS date_time)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})
# End Boost

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(AviationCalcUtil SHARED GeoUtil.cpp GeoPoint.cpp MathUtil.cpp)