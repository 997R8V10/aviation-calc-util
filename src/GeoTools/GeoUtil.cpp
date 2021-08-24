//
// Created by pshivaraman on 2021-08-19.
//

#include "GeoUtil.h"
#include "MathUtil.h"

#define _USE_MATH_DEFINES

#include <cmath>

using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::MathTools;

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

double GeoUtil::calculateDirectBearingAfterTurn(GeoPoint *aircraft, GeoPoint *waypoint, double r, double curBearing) {
    // Set waypoint's altitude to aircraft's altitude to minimize error
    waypoint->setAlt(aircraft->getAlt());

    // Get direct bearing to waypoint from aircraft. Use this to figure out right or left turn.
    bool isRightTurn = calculateTurnAmount(curBearing, GeoPoint::initialBearing(aircraft, waypoint)) > 0;

    // Calculate bearing to circle center point
    double bearingToC = normalizeHeading(isRightTurn ? curBearing + 90 : curBearing - 90);

    // Find center point
    GeoPoint *c = new GeoPoint(aircraft);
    c->moveByNMi(bearingToC, r);

    // Find distance and bearing from c to waypoint
    double finalBearingC = GeoPoint::finalBearing(c, waypoint);
    double distC = GeoPoint::distanceNMi(c, waypoint);

    // Find angle between finalBearingC and desired bearing
    double ang = MathUtil::convertRadiansToDegrees(std::asin(r / distC));

    // Calculate final bearing to waypoint
    double turnDirBearing = isRightTurn ? finalBearingC + ang : finalBearingC - ang;

    if (std::isnan(turnDirBearing)) {
        return -1;
    }

    return normalizeHeading(turnDirBearing);
}

double GeoUtil::calculateCrossTrackErrorM(GeoPoint *aircraft, GeoPoint *waypoint, double course, double &requiredCourse,
                                          double &alongTrackDistanceM) {
    // Set waypoint's altitude to aircraft's altitude to minimize error
    waypoint->setAlt(aircraft->getAlt());

    // Find radial
    double finalDirBearing = GeoPoint::finalBearing(aircraft, waypoint);
    double dist = GeoPoint::flatDistanceM(aircraft, waypoint);

    double turnAmt = calculateTurnAmount(course, finalDirBearing);

    double radial = std::fabs(turnAmt) < 90 ? normalizeHeading(course + 180) : course;

    // Calculate radius
    double R = EARTH_RADIUS_M + MathUtil::convertFeetToMeters(aircraft->getAlt());

    // Calculate angular distance between aircraft and waypoint
    double sigma13 = dist / R;

    // Initial bearing from waypoint to aircraft
    double theta13 = MathUtil::convertDegreesToRadians(normalizeHeading(finalDirBearing + 180));

    // Radial in radians
    double theta12 = MathUtil::convertDegreesToRadians(radial);

    double xTrackM = std::asin(std::sin(sigma13) * std::sin(theta13 - theta12)) * R;

    // Calculate along track distance
    double aTrackM = std::acos(std::cos(sigma13) / std::cos(xTrackM / R)) * R;

    // Find along track waypoint
    GeoPoint *aTrackPoint = new GeoPoint(waypoint);
    aTrackPoint->moveByM(radial, aTrackM);

    // Calculate required course
    double initialCourse = GeoPoint::initialBearing(aTrackPoint, waypoint);
    requiredCourse = std::fabs(turnAmt) < 90 ? initialCourse : normalizeHeading(initialCourse + 180);

    alongTrackDistanceM = std::fabs(turnAmt) < 90 ? aTrackM : -aTrackM;

    return std::fabs(turnAmt) < 90 ? -xTrackM : xTrackM;
}

double GeoUtil::calculateTurnLeadDistance(GeoPoint *point, double theta, double r) {
    if (point == nullptr) {
        return -1;
    }

    double halfAngle = MathUtil::convertDegreesToRadians(theta / 2);

    double halfTan = std::tan(halfAngle);

    // Find lead in distance
    double leadDist = r * halfTan;

    return leadDist;
}

double GeoUtil::calculateTurnLeadDistance(GeoPoint *pos, GeoPoint *wp, double trueTrack, double tas, double course,
                                          double trueWindDir, double windSpd, double &radiusOfTurn,
                                          GeoPoint &intersection) {
    // Find intersection
    intersection = findIntersection(pos, wp, trueTrack, course);

    // Find degrees to turn
    double turnAmt = calculateTurnAmount(trueTrack, course);
    double theta = std::fabs(turnAmt);

    // Calculate constant radius of turn
    double r = calculateConstantRadiusTurn(trueTrack, turnAmt, trueWindDir, windSpd, tas);

    radiusOfTurn = r;

    return calculateTurnLeadDistance(&intersection, theta, r);
}

GeoPoint *GeoUtil::findIntersection(GeoPoint *position, GeoPoint *wp, double trueTrack, double course) {
    GeoPoint *point1 = new GeoPoint(position);

    // Try both radials and see which one works
    GeoPoint *intersection1 = GeoPoint::intersection(point1, trueTrack, wp, course);
    GeoPoint *intersection2 = GeoPoint::intersection(point1, trueTrack, wp, std::fmod(course + 180, 360));

    if (intersection1 == nullptr) {
        return intersection2;
    }

    if (intersection2 == nullptr) {
        return intersection1;
    }

    double dist1 = GeoPoint::flatDistanceNMi(point1, intersection1);
    double dist2 = GeoPoint::flatDistanceNMi(point1, intersection2);

    if (dist1 < dist2) {
        return intersection1;
    }

    return intersection2;
}

double GeoUtil::calculateMaxBankAngle(double groundSpeed, double bankLimit, double turnRate) {
    double g = 1091;
    double bankAngle = MathUtil::convertRadiansToDegrees(std::atan2(turnRate * groundSpeed, g));

    return bankAngle > bankLimit ? bankLimit : bankAngle;
}

double GeoUtil::calculateRadiusOfTurn(double bankAngle, double groundSpeed) {
    // Acceleration due to gravity in NMi/hour^2
    double g = 68648.8121;
    return std::pow(groundSpeed, 2) / (g * std::tan(MathUtil::convertDegreesToRadians(bankAngle)));
}

double GeoUtil::calculateBankAngle(double radiusOfTurn, double groundSpeed) {
    // Acceleration due to gravity in NMi/hour^2
    double g = 68648.8121;
    return MathUtil::convertRadiansToDegrees(std::atan2(std::pow(groundSpeed, 2), g * radiusOfTurn));
}

double GeoUtil::calculateConstantRadiusTurn(double startBearing, double turnAmount, double windBearing,
                                            double windSpeed, double tas) {
    // Find bearing with maximum tailwind
    double tWindDir = normalizeHeading(windBearing + 180);
    double windTurnAmt = calculateTurnAmount(startBearing, tWindDir);

    // Adjust wind turn amount to match turn amount
    if (turnAmount > 0 && windTurnAmt < 0) {
        windTurnAmt += 360;
    } else if (turnAmount < 0 && windTurnAmt > 0) {
        windTurnAmt -= 360;
    }

    double maxWindBearing = windBearing;

    // Check if wind falls outside bounds
    if (windTurnAmt > turnAmount && turnAmount > 0) {
        double halfBoundary = turnAmount + (360 - (turnAmount / 2));
        if (windTurnAmt <= halfBoundary) {
            maxWindBearing = normalizeHeading(startBearing + turnAmount + 180);
        } else {
            maxWindBearing = normalizeHeading(startBearing + 180);
        }
    } else if (windTurnAmt < turnAmount && turnAmount < 0) {
        double halfBoundary = turnAmount - (-360 + (turnAmount / 2));
        if (windTurnAmt >= halfBoundary) {
            maxWindBearing = normalizeHeading(startBearing + turnAmount + 180);
        } else {
            maxWindBearing = normalizeHeading(startBearing + 180);
        }
    }

    // Get groundspeed at max wind
    double maxGs = tas + getHeadwindComponent(windSpeed, windBearing, maxWindBearing);

    // Get maximum radius of turn
    return calculateRadiusOfTurn(calculateMaxBankAngle(maxGs, 25, 3), maxGs);
}

double GeoUtil::getHeadwindComponent(double windSpeed, double windBearing, double bearing) {
    return windSpeed * std::cos(MathUtil::convertDegreesToRadians(bearing - windBearing));
}

double GeoUtil::calculateDistanceTravelledNMi(double groundSpeedKts, double timeMs) {
    return (timeMs / 1000.0) * (groundSpeedKts / 60.0 / 60.0);
}

double GeoUtil::calculateDegreesTurned(double distTravelledNMi, double radiusOfTurnNMi) {
    return MathUtil::convertRadiansToDegrees(distTravelledNMi / radiusOfTurnNMi);
}

double GeoUtil::calculateEndHeading(double startHeading, double degreesTurned, bool isRightTurn) {
    double newHeading = startHeading;

    if (isRightTurn) {
        newHeading += degreesTurned;
    } else {
        newHeading -= degreesTurned;
    }

    return normalizeHeading(newHeading);
}

tuple<double, double> GeoUtil::calculateChordHeadingAndDistance(double startHeading, double degreesTurned,
                                                                double radiusOfTurnNMi, bool isRightTurn) {
    double chordLengthNMi = 2 * radiusOfTurnNMi * std::sin(degreesTurned * M_PI / (180.0 * 2));
    double chordHeading = startHeading;

    if (isRightTurn)
    {
        chordHeading += (degreesTurned / 2);
    }
    else
    {
        chordHeading -= (degreesTurned / 2);
    }

    chordHeading = normalizeHeading(chordHeading);

    return {chordHeading, chordLengthNMi};
}

double GeoUtil::calculateAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa) {
    double pressDiff = pres_set_hpa - sfc_pres_hpa;
    return alt_ind_ft - (STD_PRES_DROP * pressDiff);
}

double GeoUtil::calculateIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa) {
    double pressDiff = pres_set_hpa - sfc_pres_hpa;
    return alt_abs_ft + (STD_PRES_DROP * pressDiff);
}

double GeoUtil::calculatePressureAlt(double alt_ind_ft, double pres_set_hpa) {
    double pressDiff = pres_set_hpa - STD_PRES_HPA;
    return alt_ind_ft - (STD_PRES_DROP * pressDiff);
}

double GeoUtil::calculateIsaTemp(double alt_pres_ft) {
    if (alt_pres_ft >= 36000)
    {
        return -56.5;
    }

    return STD_TEMP_C - (alt_pres_ft * STD_LAPSE_RATE);
}

double GeoUtil::calculateDensityAlt(double alt_pres_ft, double sat) {
    double isaDev = sat - calculateIsaTemp(alt_pres_ft);

    return alt_pres_ft + (118.8 * isaDev);
}

double GeoUtil::calculateTAS(double ias, double pres_set_hpa, double alt_ind_ft, double sat) {
    double daStdTemp = MathUtil::CONV_FACTOR_KELVIN_C + STD_TEMP_C - (calculateDensityAlt(calculatePressureAlt(alt_ind_ft, pres_set_hpa), sat) * STD_LAPSE_RATE);
    double tempRatio = daStdTemp / (STD_TEMP_C + MathUtil::CONV_FACTOR_KELVIN_C);
    double densityRatio = std::pow(tempRatio, 1 / 0.234969);
    double tasCoeff = 1 / (std::sqrt(densityRatio));

    return tasCoeff * ias;
}

double GeoUtil::calculateTurnAmount(double currentHeading, double desiredHeading) {
    // Either distance or 360 - distance
    double phi = std::fmod(std::fabs(desiredHeading - currentHeading), 360);

    double distance = phi > 180 ? 360 - phi : phi;

    // Figure out if left turn or right turn
    int sign = 1;
    if ((currentHeading - desiredHeading >= 0 && currentHeading - desiredHeading <= 180) ||
    (currentHeading - desiredHeading <= -180 && currentHeading - desiredHeading >= -360))
    {
        sign = -1;
    }

    return distance * sign;
}
