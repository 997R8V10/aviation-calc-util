//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICUTIL_H
#define AVIATION_CALC_UTIL_MAGNETICUTIL_H

#ifdef _LIBRARY
#include "MagneticModel.h"
#include <mutex>
#endif

#include "MagneticResult.h"
#include "GeoPoint.h"
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace AviationCalcUtil::GeoTools;

namespace AviationCalcUtil::GeoTools::MagneticTools{
    class MagneticUtil {
#ifdef _LIBRARY
    private:
        static MagneticModel *model;
        static std::mutex modelLock;
#endif

    public:
        static const double EARTH_WGS84_SEMI_MAJOR_AXIS;
        static const double EARTH_WGS84_RECIPROCAL_FLATTENING;
        static const int WMM_EXPANSION;
        static const double GEOMAGNETIC_REFERENCE_RADIUS;

        static void loadData();
        static MagneticResult *getMagneticField(GeoPoint *point, boost::gregorian::date date);
        static double getEpochYear(boost::gregorian::date date);
        static void getSpherical(GeoPoint *point, double &lambda, double &phiPrime, double &r);
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICUTIL_H
