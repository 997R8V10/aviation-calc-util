//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_GEOUTIL_H
#define AVIATION_SIM_UTIL_GEOUTIL_H

#define _USE_MATH_DEFINES
#include <cmath>

namespace AviationCalcUtil::GeoTools{
    class GeoUtil {
    public:
        static const double EARTH_RADIUS_M;
        static const double STD_PRES_HPA;
        static const double STD_TEMP_C;
        static const double STD_LAPSE_RATE;
        static const double STD_PRES_DROP;

        static double normalizeLongitude(double lon);
        static double normalizeHeading(double hdg);
    };
}

#endif //AVIATION_SIM_UTIL_GEOUTIL_H
