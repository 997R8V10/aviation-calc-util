//
// Created by pshivaraman on 2021-08-19.
//

#define _USE_MATH_DEFINES

#include <cmath>

#include "GeoTools/GeoPoint.h"
#include <limits>
#include <memory>
#include "MathTools/MathUtil.h"
#include "GeoTools/GeoUtil.h"

using namespace AviationCalcUtil::MathTools;
using namespace AviationCalcUtil::GeoTools;

GeoPoint::GeoPoint(const GeoPoint &point) {
    lat = point.getLat();
    lon = point.getLon();
    alt = point.getAlt();
}

GeoPoint::GeoPoint(double lat, double lon, double alt) {
    this->lat = std::fmin(std::fmax(lat, -90), 90);
    this->lon = GeoUtil::normalizeLongitude(lon);
    this->alt = alt;
}

void GeoPoint::moveByM(double bearing, double distance) {
    double R = GeoUtil::EARTH_RADIUS_M + MathUtil::convertFeetToMeters(alt);
    double bearingRads = MathUtil::convertDegreesToRadians(GeoUtil::normalizeHeading(bearing));
    double lat1 = MathUtil::convertDegreesToRadians(lat);
    double lon1 = MathUtil::convertDegreesToRadians(lon);

    double lat2 = std::asin((std::sin(lat1) * std::cos(distance / R) +
                             std::cos(lat1) * std::sin(distance / R) * std::cos(bearingRads)));
    double lon2 = lon1 + std::atan2(std::sin(bearingRads) * std::sin(distance / R) * std::cos(lat1),
                                    std::cos(distance / R) - std::sin(lat1) * std::sin(lat2));

    setLat(MathUtil::convertRadiansToDegrees(lat2));
    setLon(MathUtil::convertRadiansToDegrees(lon2));
}

void GeoPoint::moveByNMi(double bearing, double distance) {
    moveByM(bearing, MathUtil::convertNauticalMilesToMeters(distance));
}

double GeoPoint::flatDistanceM(const GeoPoint &point1, const GeoPoint &point2) {
    double phi1 = MathUtil::convertDegreesToRadians(point1.getLat());
    double phi2 = MathUtil::convertDegreesToRadians(point2.getLat());
    double deltaPhi = MathUtil::convertDegreesToRadians(point2.getLat() - point1.getLat());
    double deltaLambda = MathUtil::convertDegreesToRadians(point2.getLon() - point1.getLon());

    double a = std::pow(std::sin(deltaPhi / 2), 2) +
               std::cos(phi1) * std::cos(phi2) *
               std::pow(std::sin(deltaLambda / 2), 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    double d = GeoUtil::EARTH_RADIUS_M * c;

    return d;
}

double GeoPoint::flatDistanceNMi(const GeoPoint &point1, const GeoPoint &point2) {
    return MathUtil::convertMetersToNauticalMiles(flatDistanceM(point1, point2));
}

double GeoPoint::distanceM(const GeoPoint &point1, const GeoPoint &point2) {
    double flatDist = flatDistanceM(point1, point2);
    double altDist = MathUtil::convertFeetToMeters(std::fabs(point2.getAlt() - point1.getAlt()));

    return std::sqrt(std::pow(flatDist, 2) + std::pow(altDist, 2));
}

double GeoPoint::distanceNMi(const GeoPoint &point1, const GeoPoint &point2) {
    return MathUtil::convertMetersToNauticalMiles(distanceM(point1, point2));
}

std::unique_ptr<GeoPoint>
GeoPoint::intersection(const GeoPoint &point1, double bearing1, const GeoPoint &point2, double bearing2) {
    // Conversions to radians
    double phi1 = MathUtil::convertDegreesToRadians(point1.getLat());
    double phi2 = MathUtil::convertDegreesToRadians(point2.getLat());
    double lambda1 = MathUtil::convertDegreesToRadians(point1.getLon());
    double lambda2 = MathUtil::convertDegreesToRadians(point2.getLon());
    double theta13 = MathUtil::convertDegreesToRadians(bearing1);
    double theta23 = MathUtil::convertDegreesToRadians(bearing2);
    double deltaPhi = phi2 - phi1;
    double deltaLambda = lambda2 - lambda1;

    // Angular distance (lat1, lon1) - (lat2, lon2)
    double sigma12 = 2 * std::asin(std::sqrt(std::pow(std::sin(deltaPhi / 2), 2)
                                             +
                                             std::cos(phi1) * std::cos(phi2) * std::pow(std::sin(deltaLambda / 2), 2)));

    // Coincident points
    if (sigma12 < std::numeric_limits<double>::epsilon()) {
        return std::make_unique<GeoPoint>(point1);
    }

    // Initial/Final Bearing between points
    double cosThetaA = (std::sin(phi2) - std::sin(phi1) * std::cos(sigma12)) / (std::sin(sigma12) * std::cos(phi1));
    double cosThetaB = (std::sin(phi1) - std::sin(phi2) * std::cos(sigma12)) / (std::sin(sigma12) * std::cos(phi2));
    double thetaA = std::acos(std::fmin(std::fmax(cosThetaA, -1), 1)); // Prevent rounding errors
    double thetaB = std::acos(std::fmin(std::fmax(cosThetaB, -1), 1)); // Prevent rounding errors

    double theta12 = std::sin(deltaLambda) > 0 ? thetaA : 2 * M_PI - thetaA;
    double theta21 = std::sin(deltaLambda) > 0 ? 2 * M_PI - thetaB : thetaB;

    double alpha1 = theta13 - theta12;// Angle 2-1-3
    double alpha2 = theta21 - theta23;// Angle 1-2-3

    // Infinite intersections
    if (std::sin(alpha1) == 0 && std::sin(alpha2) == 0) {
        return nullptr;
    }

    // Ambiguous Intersection (antipodal?)
    if (std::sin(alpha1) * std::sin(alpha2) < 0) {
        return nullptr;
    }

    double cosAlpha3 = -std::cos(alpha1) * std::cos(alpha2) + std::sin(alpha1) * std::sin(alpha2) * std::cos(sigma12);

    double sigma13 = std::atan2(std::sin(sigma12) * std::sin(alpha1) * std::sin(alpha2),
                                std::cos(alpha2) + std::cos(alpha1) * cosAlpha3);

    double phi3 = std::asin(std::fmin(
            std::fmax(std::sin(phi1) * std::cos(sigma13) + std::cos(phi1) * std::sin(sigma13) * std::cos(theta13), -1),
            1));

    double deltaLambda13 = std::atan2(std::sin(theta13) * std::sin(sigma13) * std::cos(phi1),
                                      std::cos(sigma13) - std::sin(phi1) * std::sin(phi3));

    double lambda3 = lambda1 + deltaLambda13;

    return std::make_unique<GeoPoint>(MathUtil::convertRadiansToDegrees(phi3),
                                      MathUtil::convertRadiansToDegrees(lambda3));
}

double GeoPoint::initialBearing(const GeoPoint &point1, const GeoPoint &point2) {
    // Convert to Radians
    double phi1 = MathUtil::convertDegreesToRadians(point1.getLat());
    double phi2 = MathUtil::convertDegreesToRadians(point2.getLat());
    double lambda1 = MathUtil::convertDegreesToRadians(point1.getLon());
    double lambda2 = MathUtil::convertDegreesToRadians(point2.getLon());

    // Find angle between the two
    double y = std::sin(lambda2 - lambda1) * std::cos(phi2);
    double x = std::cos(phi1) * std::sin(phi2) - std::sin(phi1) * std::cos(phi2) * std::cos(lambda2 - lambda1);

    double theta = std::atan2(y, x);

    // Convert from -180, +180 to 0, 359
    return GeoUtil::normalizeHeading(MathUtil::convertRadiansToDegrees(theta));
}

double GeoPoint::finalBearing(const GeoPoint &point1, const GeoPoint &point2) {
    // Calculate initial bearing from end to start and reverse
    return std::fmod(initialBearing(point2, point1) + 180, 360);
}

bool GeoPoint::equals(const GeoPoint &o) const {
    return lat == o.getLat() && lon == o.getLon();
}

double GeoPoint::getLat() const {
    return lat;
}

void GeoPoint::setLat(double newLat) {
    lat = std::fmin(std::fmax(newLat, -90), 90);
}

double GeoPoint::getLon() const {
    return lon;
}

void GeoPoint::setLon(double newLon) {
    lon = GeoUtil::normalizeLongitude(newLon);
}

double GeoPoint::getAlt() const {
    return alt;
}

void GeoPoint::setAlt(double newAlt) {
    alt = newAlt;
}

double GeoPoint::operator-(const GeoPoint &point) const {
    return distanceM(*this, point);
}

/*
AviationCalcUtil::GeoTools::GeoPoint *CreateGeoPoint(double lat, double lon, double alt) {
    return new GeoPoint(lat, lon, alt);
}

AviationCalcUtil::GeoTools::GeoPoint *CopyGeoPoint(AviationCalcUtil::GeoTools::GeoPoint* ptr) {
    return new AviationCalcUtil::GeoTools::GeoPoint(*ptr);
}

void DisposeGeoPoint(AviationCalcUtil::GeoTools::GeoPoint *ptr) {
    if (ptr != NULL) {
        delete ptr;
        ptr = NULL;
    }
}

void GeoPointMoveByM(AviationCalcUtil::GeoTools::GeoPoint *ptr, double bearing, double distance) {
    if (ptr != NULL) {
        ptr->moveByM(bearing, distance);
    }
}

void GeoPointMoveByNMi(AviationCalcUtil::GeoTools::GeoPoint *ptr, double bearing, double distance)
{
    if (ptr != NULL) {
        ptr->moveByNMi(bearing, distance);
    }
}

double GeoPointFlatDistanceM(AviationCalcUtil::GeoTools::GeoPoint *ptr1, AviationCalcUtil::GeoTools::GeoPoint *ptr2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return GeoPoint::flatDistanceM(*ptr1, *ptr2);
    }
    return -1;
}

double GeoPointFlatDistanceNMi(AviationCalcUtil::GeoTools::GeoPoint *ptr1, AviationCalcUtil::GeoTools::GeoPoint *ptr2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return GeoPoint::flatDistanceNMi(*ptr1, *ptr2);
    }
    return -1;
}

double GeoPointDistanceM(AviationCalcUtil::GeoTools::GeoPoint *ptr1, AviationCalcUtil::GeoTools::GeoPoint *ptr2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return GeoPoint::distanceM(*ptr1, *ptr2);
    }
    return -1;
}

double GeoPointDistanceNMi(AviationCalcUtil::GeoTools::GeoPoint *ptr1, AviationCalcUtil::GeoTools::GeoPoint *ptr2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return GeoPoint::distanceNMi(*ptr1, *ptr2);
    }
    return -1;
}

AviationCalcUtil::GeoTools::GeoPoint*
GeoPointIntersection(AviationCalcUtil::GeoTools::GeoPoint *ptr1, double bearing1, AviationCalcUtil::GeoTools::GeoPoint *ptr2,
             double bearing2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return GeoPoint::intersection(*ptr1, bearing1, *ptr2, bearing2).release();
    }
    return NULL;
}

double GeoPointInitialBearing(AviationCalcUtil::GeoTools::GeoPoint *ptr1, AviationCalcUtil::GeoTools::GeoPoint *ptr2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return GeoPoint::initialBearing(*ptr1, *ptr2);
    }
    return -1;
}

double GeoPointFinalBearing(AviationCalcUtil::GeoTools::GeoPoint *ptr1, AviationCalcUtil::GeoTools::GeoPoint *ptr2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return GeoPoint::finalBearing(*ptr1, *ptr2);
    }
    return -1;
}

bool GeoPointEquals(AviationCalcUtil::GeoTools::GeoPoint *ptr1, AviationCalcUtil::GeoTools::GeoPoint *ptr2) {
    if (ptr1 != NULL && ptr2 != NULL)
    {
        return ptr1->equals(*ptr2);
    }
    return false;
}

double GeoPointGetLat(AviationCalcUtil::GeoTools::GeoPoint *ptr) {
    if (ptr != NULL)
    {
        return ptr->getLat();
    }
    return -1;
}

void GeoPointSetLat(AviationCalcUtil::GeoTools::GeoPoint *ptr, double newLat) {
    if (ptr != NULL)
    {
        ptr->setLat(newLat);
    }
}

double GeoPointGetLon(AviationCalcUtil::GeoTools::GeoPoint *ptr) {
    if (ptr != NULL)
    {
        return ptr->getLon();
    }
    return -1;
}

void GeoPointSetLon(AviationCalcUtil::GeoTools::GeoPoint *ptr, double newLon) {
    if (ptr != NULL)
    {
        ptr->setLon(newLon);
    }
}

double GeoPointGetAlt(AviationCalcUtil::GeoTools::GeoPoint *ptr) {
    if (ptr != NULL)
    {
        return ptr->getAlt();
    }
    return -1;
}

void GeoPointSetAlt(AviationCalcUtil::GeoTools::GeoPoint *ptr, double newAlt) {
    if (ptr != NULL)
    {
        ptr->setAlt(newAlt);
    }
}*/


