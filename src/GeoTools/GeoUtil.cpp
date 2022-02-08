//
// Created by pshivaraman on 2021-08-19.
//

#define _USE_MATH_DEFINES
#include <cmath>

#include "GeoTools/GeoUtil.h"
#include "MathTools/MathUtil.h"

using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::MathTools;



double GeoUtil::normalizeLongitude(double lon) {
    return std::fmod((lon + 540), 360.0) - 180;
}

double GeoUtil::normalizeHeading(double hdg) {
    return std::fmod(hdg + 360, 360);
}

double GeoUtil::calculateMaxBankAngle(double groundSpeed, double bankLimit, double turnRate) {
    double g = 1091;
    double bankAngle = MathUtil::convertRadiansToDegrees(std::atan2(turnRate * groundSpeed, g));

    return bankAngle > bankLimit ? bankLimit : bankAngle;
}

double GeoUtil::calculateRadiusOfTurn(double bankAngle, double groundSpeed) {
    if (groundSpeed == 0){
        return 1;
    }

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

    if (isRightTurn) {
        chordHeading += (degreesTurned / 2);
    } else {
        chordHeading -= (degreesTurned / 2);
    }

    chordHeading = normalizeHeading(chordHeading);

    return {chordHeading, chordLengthNMi};
}

double GeoUtil::calculateTurnAmount(double currentHeading, double desiredHeading) {
    // Either distance or 360 - distance
    double phi = std::fmod(std::fabs(desiredHeading - currentHeading), 360);

    double distance = phi > 180 ? 360 - phi : phi;

    // Figure out if left turn or right turn
    int sign = 1;
    if ((currentHeading - desiredHeading >= 0 && currentHeading - desiredHeading <= 180) ||
        (currentHeading - desiredHeading <= -180 && currentHeading - desiredHeading >= -360)) {
        sign = -1;
    }

    return distance * sign;
}

double GeoUtil::calculateDirectBearingAfterTurn(const GeoPoint &aircraft, const GeoPoint &waypoint, double r,
                                                double curBearing) {
    // Set waypoint's altitude to aircraft's altitude to minimize error
    GeoPoint tempWp(waypoint.getLat(), waypoint.getLon(), aircraft.getAlt());

    // Get direct bearing to waypoint from aircraft. Use this to figure out right or left turn.
    bool isRightTurn = calculateTurnAmount(curBearing, GeoPoint::initialBearing(aircraft, tempWp)) > 0;

    // Calculate bearing to circle center point
    double bearingToC = normalizeHeading(isRightTurn ? curBearing + 90 : curBearing - 90);

    // Find center point
    GeoPoint c(aircraft);
    c.moveByNMi(bearingToC, r);

    // Find distance and bearing from c to waypoint
    double finalBearingC = GeoPoint::finalBearing(c, tempWp);
    double distC = GeoPoint::distanceNMi(c, tempWp);

    // Find angle between finalBearingC and desired bearing
    double ang = MathUtil::convertRadiansToDegrees(std::asin(r / distC));

    // Calculate final bearing to waypoint
    double turnDirBearing = isRightTurn ? finalBearingC + ang : finalBearingC - ang;

    if (std::isnan(turnDirBearing)) {
        return -1;
    }

    return normalizeHeading(turnDirBearing);
}

double GeoUtil::calculateCrossTrackErrorM(const GeoPoint &aircraft, const GeoPoint &waypoint, double course,
                                          double &requiredCourse, double &alongTrackDistanceM) {
    // Set waypoint's altitude to aircraft's altitude to minimize error
    GeoPoint tempWp(waypoint.getLat(), waypoint.getLon(), aircraft.getAlt());

    // Find radial
    double finalDirBearing = GeoPoint::finalBearing(aircraft, tempWp);
    double dist = GeoPoint::flatDistanceM(aircraft, tempWp);

    double turnAmt = calculateTurnAmount(course, finalDirBearing);

    double radial = std::fabs(turnAmt) < 90 ? normalizeHeading(course + 180) : course;

    // Calculate radius
    double R = EARTH_RADIUS_M + MathUtil::convertFeetToMeters(aircraft.getAlt());

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
    GeoPoint aTrackPoint(tempWp);
    aTrackPoint.moveByM(radial, aTrackM);

    // Calculate required course
    double initialCourse = GeoPoint::initialBearing(aTrackPoint, tempWp);
    requiredCourse = std::fabs(turnAmt) < 90 ? initialCourse : normalizeHeading(initialCourse + 180);

    alongTrackDistanceM = std::fabs(turnAmt) < 90 ? aTrackM : -aTrackM;

    return std::fabs(turnAmt) < 90 ? -xTrackM : xTrackM;
}

double GeoUtil::calculateTurnLeadDistance(const GeoPoint &point, double theta, double r) {
    double halfAngle = MathUtil::convertDegreesToRadians(theta / 2);

    double halfTan = std::tan(halfAngle);

    // Find lead in distance
    double leadDist = r * halfTan;

    return leadDist;
}

double GeoUtil::calculateTurnLeadDistance(const GeoPoint &pos, const GeoPoint &wp, double trueTrack, double tas,
                                          double course, double trueWindDir, double windSpd, double &radiusOfTurn,
                                          std::unique_ptr<GeoPoint> &intersection) {
    // Find intersection
    intersection = std::move(findIntersection(pos, wp, trueTrack, course));

    // Find degrees to turn
    double turnAmt = calculateTurnAmount(trueTrack, course);
    double theta = std::fabs(turnAmt);

    // Calculate constant radius of turn
    double r = calculateConstantRadiusTurn(trueTrack, turnAmt, trueWindDir, windSpd, tas);

    radiusOfTurn = r;

    return calculateTurnLeadDistance(*intersection, theta, r);
}

std::unique_ptr<GeoPoint> GeoUtil::findIntersection(const GeoPoint &position, const GeoPoint &wp,
                                                    double trueTrack, double course) {
    GeoPoint point1(position);

    // Try both radials and see which one works
    std::unique_ptr<GeoPoint> intersection1 = std::move(GeoPoint::intersection(point1, trueTrack, wp, course));
    std::unique_ptr<GeoPoint> intersection2 = std::move(GeoPoint::intersection(point1, trueTrack, wp, std::fmod(course + 180, 360)));

    if (intersection1 == nullptr) {
        return intersection2;
    }

    if (intersection2 == nullptr) {
        return intersection1;
    }

    double dist1 = GeoPoint::flatDistanceNMi(point1, *intersection1);
    double dist2 = GeoPoint::flatDistanceNMi(point1, *intersection2);

    if (dist1 < dist2) {
        return intersection1;
    }

    return intersection2;
}

double GeoUtilCalculateDirectBearingAfterTurn(AviationCalcUtil::GeoTools::GeoPoint *aircraft,
                                              AviationCalcUtil::GeoTools::GeoPoint *waypoint, double r,
                                              double curBearing) {
    if (aircraft != NULL && waypoint != NULL)
    {
        return GeoUtil::calculateDirectBearingAfterTurn(*aircraft, *waypoint, r, curBearing);
    }
    return -1;
}

double GeoUtilCalculateCrossTrackErrorM(GeoPoint *aircraft, GeoPoint *waypoint, double course, double &requiredCourse,
                                        double &alongTrackDistanceM) {
    if (aircraft != NULL && waypoint != NULL)
    {
        return GeoUtil::calculateCrossTrackErrorM(*aircraft, *waypoint, course, requiredCourse, alongTrackDistanceM);
    }
    return -1;
}

double GeoUtilCalculateTurnLeadInDistance(GeoPoint *ptr, double theta, double r) {
    if (ptr != NULL) {
        return GeoUtil::calculateTurnLeadDistance(*ptr, theta, r);
   }
    return -1;
}

double GeoUtilCalculateTurnLeadDistance(GeoPoint *pos, GeoPoint *wp, double trueTrack, double tas, double course,
                                        double trueWindDir, double windSpd, double &radiusOfTurn,
                                        GeoPoint *&intersection) {
    if (pos != NULL && wp != NULL)
    {
        auto nat_int = std::make_unique<GeoPoint>();
        double toReturn = GeoUtil::calculateTurnLeadDistance(*pos, *wp, trueTrack, tas, course, trueWindDir, windSpd, radiusOfTurn, nat_int);
        intersection = nat_int.release();
        return toReturn;
    }
    return -1;
}

GeoPoint *GeoUtilFindIntersection(GeoPoint *position, GeoPoint *wp, double trueTrack, double course) {
    if (position != NULL && wp != NULL)
    {
        return GeoUtil::findIntersection(*position, *wp, trueTrack, course).release();
    }
    return NULL;
}

double GeoUtilNormalizeLongitude(double lon) {
    return GeoUtil::normalizeLongitude(lon);
}

double GeoUtilCalculateBankAngle(double radiusOfTurn, double groundSpeed) {
    return GeoUtil::calculateBankAngle(radiusOfTurn, groundSpeed);
}

double GeoUtilNormalizeHeading(double hdg) {
    return GeoUtil::normalizeHeading(hdg);
}

double GeoUtilCalculateMaxBankAngle(double groundSpeed, double bankLimit, double turnRate) {
    return GeoUtil::calculateMaxBankAngle(groundSpeed, bankLimit, turnRate);
}

double GeoUtilCalculateRadiusOfTurn(double bankAngle, double groundSpeed) {
    return GeoUtil::calculateRadiusOfTurn(bankAngle, groundSpeed);
}

double GeoUtilCalculateConstantRadiusTurn(double startBearing, double turnAmount, double windBearing, double windSpeed,
                                          double tas) {
    return GeoUtil::calculateConstantRadiusTurn(startBearing, turnAmount, windBearing, windSpeed, tas);
}

double GeoUtilGetHeadwindComponent(double windSpeed, double windBearing, double bearing) {
    return GeoUtil::getHeadwindComponent(windSpeed, windBearing, bearing);
}

double GeoUtilCalculateDistanceTravelledNMi(double groundSpeedKts, double timeMs) {
    return GeoUtil::calculateDistanceTravelledNMi(groundSpeedKts, timeMs);
}

double GeoUtilCalculateDegreesTurned(double distTravelledNMi, double radiusOfTurnNMi) {
    return GeoUtil::calculateDegreesTurned(distTravelledNMi, radiusOfTurnNMi);
}

double GeoUtilCalculateEndHeading(double startHeading, double degreesTurned, bool isRightTurn) {
    return GeoUtil::calculateEndHeading(startHeading, degreesTurned, isRightTurn);
}

void GeoUtilCalculateChordHeadingAndDistance(double startHeading, double degreesTurned, double radiusOfTurnNMi,
                                             bool isRightTurn, double &chordHeading, double &chordDistance) {
    tuple<double, double> answer = GeoUtil::calculateChordHeadingAndDistance(startHeading, degreesTurned, radiusOfTurnNMi, isRightTurn);
    chordHeading = std::get<0>(answer);
    chordDistance = std::get<1>(answer);
}

double GeoUtilCalculateTurnAmount(double currentHeading, double desiredHeading) {
    return GeoUtil::calculateTurnAmount(currentHeading, desiredHeading);
}

double GeoUtilGetEarthRadiusM() {
    return GeoUtil::EARTH_RADIUS_M;
}
