//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICUTIL_H
#define AVIATION_CALC_UTIL_MAGNETICUTIL_H

#include "MagneticResult.h"
#include "GeoPoint.h"
#include "boost/date_time/gregorian/gregorian.hpp"

#ifdef _LIBRARY

#include "MagneticModel.h"
#include <mutex>
#include <memory>

#endif

using namespace AviationCalcUtil::GeoTools;

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticUtil {
#ifdef _LIBRARY
    private:
        inline static std::shared_ptr<MagneticModel> model;
        static std::mutex modelLock;
#endif

    public:
        static constexpr double EARTH_WGS84_SEMI_MAJOR_AXIS = 6378137.0;
        static constexpr double EARTH_WGS84_RECIPROCAL_FLATTENING = 298.257223563;
        static constexpr int WMM_EXPANSION = 12;
        static constexpr double GEOMAGNETIC_REFERENCE_RADIUS = 6371200.0;

        static void loadData();

        static std::unique_ptr<MagneticResult>
        getMagneticField(const GeoPoint &point, const boost::gregorian::date &date);

        static double getEpochYear(const boost::gregorian::date &date);

        static void getSpherical(const GeoPoint &point, double &lambda, double &phiPrime, double &r);

        /// Converts Magnetic Bearings to True Bearings with declination
        /// \param magneticBearing Magnetic Bearing (degrees)
        /// \param declination Magnetic Declination (degrees)
        /// \return True Bearing (degrees)
        static double convertMagneticToTrue(double magneticBearing, double declination);

        /// Converts Magnetic Bearings to True Bearings at a position
        /// \param magneticBearing Magnetic Bearing (degrees)
        /// \param position Position
        /// \return True Bearing (degrees)
        static double convertMagneticToTrue(double magneticBearing, const GeoPoint &position);

        /// Converts True Bearings to Magnetic Bearings with declination
        /// \param trueBearing True Bearing (degrees)
        /// \param declination Magnetic Declination (degrees)
        /// \return Magnetic Bearing (degrees)
        static double convertTrueToMagnetic(double trueBearing, double declination);

        /// Converts True Bearings to Magnetic Bearings at a position
        /// \param trueBearing True Bearing (degrees)
        /// \param position Position
        /// \return Magnetic Bearing (degrees)
        static double convertTrueToMagnetic(double trueBearing, const GeoPoint &position);
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICUTIL_H
