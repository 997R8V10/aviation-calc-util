//
// Created by pshivaraman on 2021-08-19.
//

#include "GeoUtil.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace AviationCalcUtil::GeoTools;

const double GeoUtil::EARTH_RADIUS_M = 6371e3;
const double GeoUtil::STD_PRES_HPA = 1013.25;
const double GeoUtil::STD_TEMP_C = 15;
const double GeoUtil::STD_LAPSE_RATE = 2.0 / 1000.0;
const double GeoUtil::STD_PRES_DROP = 30.0;

double GeoUtil::normalizeLongitude(double lon) {
    return std::fmod((lon + 540), 360.0) - 180;
}

double GeoUtil::normalizeHeading(double hdg) {
    return std::fmod(hdg + 360, 360);
}
