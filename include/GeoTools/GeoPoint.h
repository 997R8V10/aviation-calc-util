//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_GEOPOINT_H
#define AVIATION_SIM_UTIL_GEOPOINT_H
#define _USE_MATH_DEFINES
#include <cmath>

#include <limits>
#include "MathUtil.h"
#include "GeoUtil.h"

namespace AviationCalcUtil::GeoTools{
    class GeoPoint {
    private:
        double lat;
        double lon;
        double alt;

    public:
        // Constructors
        GeoPoint(GeoPoint *point);
        GeoPoint(double lat, double lon, double alt);
        GeoPoint(double lat, double lon);

        // Methods
        void moveByM(double bearing, double distance);
        void moveByNMi(double bearing, double distance);
        static double flatDistanceM(GeoPoint *point1, GeoPoint *point2);
        static double flatDistanceNMi(GeoPoint *point1, GeoPoint *point2);
        static double distanceM(GeoPoint *point1, GeoPoint *point2);
        static double distanceNMi(GeoPoint *point1, GeoPoint *point2);
        static GeoPoint *intersection(GeoPoint *point1, double bearing1, GeoPoint *point2, double bearing2);
        static double initialBearing(GeoPoint *point1, GeoPoint *point2);
        static double finalBearing(GeoPoint *point1, GeoPoint *point2);
        bool equals(GeoPoint *o);

        // Getters/Setters
        double getLat();
        void setLat(double newLat);
        double getLon();
        void setLon(double newLon);
        double getAlt();
        void setAlt(double newAlt);
    };
}

#endif //AVIATION_SIM_UTIL_GEOPOINT_H
