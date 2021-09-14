//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_GEOPOINT_H
#define AVIATION_SIM_UTIL_GEOPOINT_H

#include <memory>
#include "aviationcalc_exports.h"

namespace AviationCalcUtil::GeoTools {
    class AVIATIONCALC_EXPORT GeoPoint {
    private:
        double lat;
        double lon;
        double alt;

    public:
        // Constructors
        GeoPoint(const GeoPoint &point);

        explicit GeoPoint(double lat = 0, double lon = 0, double alt = 0);

        // Methods
        void moveByM(double bearing, double distance);

        void moveByNMi(double bearing, double distance);

        static double flatDistanceM(const GeoPoint &point1, const GeoPoint &point2);

        static double flatDistanceNMi(const GeoPoint &point1, const GeoPoint &point2);

        static double distanceM(const GeoPoint &point1, const GeoPoint &point2);

        static double distanceNMi(const GeoPoint &point1, const GeoPoint &point2);

        static std::unique_ptr<GeoPoint>
        intersection(const GeoPoint &point1, double bearing1, const GeoPoint &point2, double bearing2);

        double operator-(const GeoPoint &point) const;

        static double initialBearing(const GeoPoint &point1, const GeoPoint &point2);

        static double finalBearing(const GeoPoint &point1, const GeoPoint &point2);

        bool equals(const GeoPoint &o) const;

        // Getters/Setters
        double getLat() const;

        void setLat(double newLat);

        double getLon() const;

        void setLon(double newLon);

        double getAlt() const;

        void setAlt(double newAlt);
    };
}

extern "C"
{
extern AVIATIONCALC_EXPORT AviationCalcUtil::GeoTools::GeoPoint *CreateGeoPoint(double lat, double lon, double alt);
extern AVIATIONCALC_EXPORT void DisposeGeoPoint(AviationCalcUtil::GeoTools::GeoPoint* ptr);
extern AVIATIONCALC_EXPORT void GeoPointMoveByM(AviationCalcUtil::GeoTools::GeoPoint* ptr, double bearing, double distance);
extern AVIATIONCALC_EXPORT void GeoPointMoveByNMi(AviationCalcUtil::GeoTools::GeoPoint* ptr, double bearing, double distance);
extern AVIATIONCALC_EXPORT double GeoPointFlatDistanceM(AviationCalcUtil::GeoTools::GeoPoint* ptr1, AviationCalcUtil::GeoTools::GeoPoint* ptr2);
extern AVIATIONCALC_EXPORT double GeoPointFlatDistanceNMi(AviationCalcUtil::GeoTools::GeoPoint* ptr1, AviationCalcUtil::GeoTools::GeoPoint* ptr2);
extern AVIATIONCALC_EXPORT double GeoPointDistanceM(AviationCalcUtil::GeoTools::GeoPoint* ptr1, AviationCalcUtil::GeoTools::GeoPoint* ptr2);
extern AVIATIONCALC_EXPORT double GeoPointDistanceNMi(AviationCalcUtil::GeoTools::GeoPoint* ptr1, AviationCalcUtil::GeoTools::GeoPoint* ptr2);
extern AVIATIONCALC_EXPORT AviationCalcUtil::GeoTools::GeoPoint* GeoPointIntersection(AviationCalcUtil::GeoTools::GeoPoint* ptr1, double bearing1, AviationCalcUtil::GeoTools::GeoPoint* ptr2, double bearing2);
extern AVIATIONCALC_EXPORT double GeoPointInitialBearing(AviationCalcUtil::GeoTools::GeoPoint* ptr1, AviationCalcUtil::GeoTools::GeoPoint* ptr2);
extern AVIATIONCALC_EXPORT double GeoPointFinalBearing(AviationCalcUtil::GeoTools::GeoPoint* ptr1, AviationCalcUtil::GeoTools::GeoPoint* ptr2);
extern AVIATIONCALC_EXPORT bool GeoPointEquals(AviationCalcUtil::GeoTools::GeoPoint* ptr1, AviationCalcUtil::GeoTools::GeoPoint* ptr2);
extern AVIATIONCALC_EXPORT double GeoPointGetLat(AviationCalcUtil::GeoTools::GeoPoint* ptr);
extern AVIATIONCALC_EXPORT void GeoPointSetLat(AviationCalcUtil::GeoTools::GeoPoint* ptr, double newLat);
extern AVIATIONCALC_EXPORT double GeoPointGetLon(AviationCalcUtil::GeoTools::GeoPoint* ptr);
extern AVIATIONCALC_EXPORT void GeoPointSetLon(AviationCalcUtil::GeoTools::GeoPoint* ptr, double newLon);
extern AVIATIONCALC_EXPORT double GeoPointGetAlt(AviationCalcUtil::GeoTools::GeoPoint* ptr);
extern AVIATIONCALC_EXPORT void GeoPointSetAlt(AviationCalcUtil::GeoTools::GeoPoint* ptr, double newAlt);
}
#endif //AVIATION_SIM_UTIL_GEOPOINT_H
