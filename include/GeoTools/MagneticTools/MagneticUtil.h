//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICUTIL_H
#define AVIATION_CALC_UTIL_MAGNETICUTIL_H

#include "GeoTools/MagneticTools/MagneticResult.h"
#include "GeoTools/GeoPoint.h"
#include <boost/date_time/gregorian/gregorian.hpp>
//#include "InteropTools/InteropUtil.h"

#ifdef _LIBRARY
#include "MagneticModel.h"
#include <mutex>
#include <memory>
#include "aviationcalc_exports.h"
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
        AVIATIONCALC_EXPORT static constexpr double EARTH_WGS84_SEMI_MAJOR_AXIS = 6378137.0;
        AVIATIONCALC_EXPORT static constexpr double EARTH_WGS84_RECIPROCAL_FLATTENING = 298.257223563;
        AVIATIONCALC_EXPORT static constexpr int WMM_EXPANSION = 12;
        AVIATIONCALC_EXPORT static constexpr double GEOMAGNETIC_REFERENCE_RADIUS = 6371200.0;

        AVIATIONCALC_EXPORT static void loadData();

        AVIATIONCALC_EXPORT static std::shared_ptr<MagneticResult>
        getMagneticField(const GeoPoint &point, const boost::gregorian::date &date);

        AVIATIONCALC_EXPORT static double getEpochYear(const boost::gregorian::date &date);

        AVIATIONCALC_EXPORT static void getSpherical(const GeoPoint &point, double &lambda, double &phiPrime, double &r);

        /// Converts Magnetic Bearings to True Bearings with declination
        /// \param magneticBearing Magnetic Bearing (degrees)
        /// \param declination Magnetic Declination (degrees)
        /// \return True Bearing (degrees)
        AVIATIONCALC_EXPORT static double convertMagneticToTrue(double magneticBearing, double declination);

        /// Converts Magnetic Bearings to True Bearings at a position
        /// \param magneticBearing Magnetic Bearing (degrees)
        /// \param position Position
        /// \return True Bearing (degrees)
        AVIATIONCALC_EXPORT static double convertMagneticToTrue(double magneticBearing, const GeoPoint &position);

        /// Converts True Bearings to Magnetic Bearings with declination
        /// \param trueBearing True Bearing (degrees)
        /// \param declination Magnetic Declination (degrees)
        /// \return Magnetic Bearing (degrees)
        AVIATIONCALC_EXPORT static double convertTrueToMagnetic(double trueBearing, double declination);

        /// Converts True Bearings to Magnetic Bearings at a position
        /// \param trueBearing True Bearing (degrees)
        /// \param position Position
        /// \return Magnetic Bearing (degrees)
        AVIATIONCALC_EXPORT static double convertTrueToMagnetic(double trueBearing, const GeoPoint &position);

        /// Converts Magnetic Bearings to True Bearings at a position using cached tiles for improved performance.
        /// Use this if you need to perform multiple magnetic calculations
        /// \param magneticBearing Magnetic Bearing (degrees)
        /// \param position Position
        /// \return True Bearing (degrees)
        AVIATIONCALC_EXPORT static double convertMagneticToTrueTile(double magneticBearing, const GeoPoint &position);

        /// Converts True Bearings to Magnetic Bearings at a position using cached tiles for improved performance.
        /// Use this if you need to perform multiple magnetic calculations
        /// \param trueBearing True Bearing (degrees)
        /// \param position Position
        /// \return Magnetic Bearing (degrees)
        AVIATIONCALC_EXPORT static double convertTrueToMagneticTile(double trueBearing, const GeoPoint &position);
    };
}
/*
extern "C"{
    extern AVIATIONCALC_EXPORT double MagneticUtilGetConst_EARTH_WGS84_SEMI_MAJOR_AXIS();
    extern AVIATIONCALC_EXPORT double MagneticUtilGetConst_EARTH_WGS84_RECIPROCAL_FLATTENING();
    extern AVIATIONCALC_EXPORT int MagneticUtilGetConst_WMM_EXPANSION();
    extern AVIATIONCALC_EXPORT double MagneticUtilGetConst_GEOMAGNETIC_REFERENCE_RADIUS();
    extern AVIATIONCALC_EXPORT void MagneticUtilLoadData();
    extern AVIATIONCALC_EXPORT std::shared_ptr<const MagneticResult> *MagneticUtilGetMagneticField(GeoPoint *point, InteropDateStruct dStruct);
    extern AVIATIONCALC_EXPORT double MagneticUtilGetEpochYear(InteropDateStruct dStruct);
    extern AVIATIONCALC_EXPORT void MagneticUtilGetSpherical(GeoPoint *point, double &lambda, double &phiPrime, double &r);
    extern AVIATIONCALC_EXPORT double MagneticUtilConvertMagneticToTrue1(double magneticBearing, double declination);
    extern AVIATIONCALC_EXPORT double MagneticUtilConvertMagneticToTrue2(double magneticBearing, GeoPoint *position);
    extern AVIATIONCALC_EXPORT double MagneticUtilConvertTrueToMagnetic1(double trueBearing, double declination);
    extern AVIATIONCALC_EXPORT double MagneticUtilConvertTrueToMagnetic2(double trueBearing, GeoPoint *position);
    extern AVIATIONCALC_EXPORT double MagneticUtilConvertMagneticToTrueTile(double magneticBearing, GeoPoint *position);
    extern AVIATIONCALC_EXPORT double MagneticUtilConvertTrueToMagneticTile(double trueBearing, GeoPoint *position);
}
*/
#endif //AVIATION_CALC_UTIL_MAGNETICUTIL_H
