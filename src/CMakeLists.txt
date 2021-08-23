# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.77.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
# End Boost

include_directories(
        ${PROJECT_SOURCE_DIR}/include/GeoTools
        ${PROJECT_SOURCE_DIR}/include/MathTools
        ${PROJECT_SOURCE_DIR}/include/Timers
)

add_library(AviationCalcUtil SHARED
        GeoTools/GeoUtil.cpp
        GeoTools/GeoPoint.cpp
        MathTools/MathUtil.cpp
        MathTools/LegendreUtil.cpp
        MathTools/Polynomial.cpp)