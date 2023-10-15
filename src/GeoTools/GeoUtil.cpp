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
    if (groundSpeed == 0) {
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

double GeoUtil::calculateDeltaToHeading(double currentHeading, double desiredHeading, bool isRightTurn) {
    double delta = isRightTurn ? desiredHeading - currentHeading : currentHeading - desiredHeading;

    // If it passes through 360, normalize it.
    if (delta < 0) {
        delta = 360 + delta;
    }

    return delta;
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

double GeoUtil::calculateArcCourseInfo(const GeoPoint &aircraft, const GeoPoint &arcCenter, double startRadial,
                                       double endRadial, double radiusM, bool clockwise, double &requiredCourse,
                                       double &alongTrackDistanceM) {
    // Set waypoint's altitude to aircraft's altitude to minimize error
    GeoPoint tempWp(arcCenter.getLat(), arcCenter.getLon(), aircraft.getAlt());

    // Calculate required course
    double radial = GeoPoint::initialBearing(tempWp, aircraft);
    requiredCourse = GeoUtil::normalizeHeading(clockwise ? radial + 90 : radial - 90);

    // Calculate Cross Track Error
    double curRadiusM = GeoPoint::flatDistanceM(tempWp, aircraft);
    double xTrackM = clockwise ? radiusM - curRadiusM : curRadiusM - radiusM;

    // Calculate Along Track Distance
    double deltaStartToEndRadial = GeoUtil::calculateDeltaToHeading(startRadial, endRadial, clockwise);
    double deltaPposToEndRadial = GeoUtil::calculateDeltaToHeading(radial, endRadial, clockwise);

    // Calculate crossover amount to divide up the rest of the circle
    double minDistance = deltaStartToEndRadial + 0.5 * (360 - deltaStartToEndRadial);

    // If end radial has been passed
    if (deltaPposToEndRadial > minDistance) {
        alongTrackDistanceM = -1;
    } else {
        // Calculate arc length in meters
        alongTrackDistanceM = 2 * M_PI * radiusM * deltaPposToEndRadial / 360.0; // 2 * pi * r * theta / 360
    }

    return xTrackM;
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
    std::unique_ptr<GeoPoint> intersection2 = std::move(
            GeoPoint::intersection(point1, trueTrack, wp, std::fmod(course + 180, 360)));

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

double GeoUtil::convertDegMinSecToDecimalDegs(int degrees, unsigned int mins, double secs) {
    int sign = (degrees < 0) ? -1 : 1;
    degrees *= sign;
    return sign * (degrees + (mins / 60.0) + (secs / 3600.0));
}

void GeoUtil::convertDecimalDegsToDegMinSec(double decimalDegs, int &degrees, unsigned int &mins, double &secs) {
    int sign = (decimalDegs < 0) ? -1 : 1;
    decimalDegs *= sign;

    degrees = (int) floor(decimalDegs);
    mins = (unsigned int) ((decimalDegs - degrees) * 60.0);
    secs = (decimalDegs - degrees - (mins / 60.0)) * 3600.0;

    degrees *= sign;
}

void GeoUtil::convertNatsToDecimalDegs(const string &natsLat, const string &natsLon, double &decimalLat,
                                       double &decimalLon) {
    decimalLat = convertNatsToDecimalSingle(natsLat, true);
    decimalLon = convertNatsToDecimalSingle(natsLon, false);
}

double GeoUtil::convertNatsToDecimalSingle(const string &natsCoord, bool isLatitude) {
    int length = isLatitude ? 7 : 8;
    string lastChar = natsCoord.substr(length - 1, 1);
    int sign = lastChar == "S" || lastChar == "W" ? -1 : 1;
    int currentPos = 0, firstChunk = isLatitude ? 2 : 3;
    int degrees = (stoi(natsCoord.substr(currentPos, firstChunk))) * sign;
    currentPos += firstChunk;
    unsigned int minutes = stoi(natsCoord.substr(currentPos, 2));
    currentPos += 2;
    double seconds = stod(natsCoord.substr(currentPos, 2));
    return convertDegMinSecToDecimalDegs(degrees, minutes, seconds);
}

void GeoUtil::convertDecimalDegsToNats(double decimalLat, double decimalLon, string &natsLat, string &natsLon) {
    natsLat = convertDecimalToNatsSingle(decimalLat, true);
    natsLon = convertDecimalToNatsSingle(decimalLon, false);
}


string GeoUtil::convertDecimalToNatsSingle(double decimalCoord, bool isLatitude) {
    int degrees;
    unsigned int minutes;
    double seconds;
    convertDecimalDegsToDegMinSec(decimalCoord, degrees, minutes, seconds);

//Determine which letter to append at the end of the return value
    char dirLetter;
    if (degrees < 0) dirLetter = isLatitude ? 'S' : 'W';
    else dirLetter = isLatitude ? 'N' : 'E';
    degrees = abs(degrees);

    //Determine how long the return value is to be
    int returnLength = isLatitude ? 7 : 8;

    //Round seconds
    seconds = round(seconds);
    if (seconds >= 60) {
        seconds = 0;
        minutes++;
        if (minutes >= 60) {
            minutes = 0;
            degrees++;
            if (isLatitude && degrees > 90) {
                degrees = 90 - (degrees - 90);
                if (dirLetter == 'N') dirLetter = 'S';
                else dirLetter = 'N';
            } else if (!isLatitude && degrees > 180) {
                degrees = 180 - (degrees - 180);
                if (dirLetter == 'E') dirLetter = 'W';
                else dirLetter = 'E';
            }
        }
    }
    int secondsI = seconds;

    //Set up an empty char* of the appropriate size to hold the return value before it is converted to a std::string
    char *value = new char[returnLength];

    //format the return value string with the appropriate level of zero-padding
    if (isLatitude) {
        sprintf(value, "%02d%02u%02d%c", degrees, minutes, secondsI, dirLetter);
    } else {
        sprintf(value, "%03d%02u%02d%c", degrees, minutes, secondsI, dirLetter);
    }

    //Convert the formatted char* to a std::string
    string natsCoord(value, returnLength);

    return natsCoord;
}

string GeoUtil::convertDecimalDegToVrcSingle(double decimalCoord, bool isLatitude) {
    //Convert to Degrees Minutes and Seconds first
    int degrees;
    unsigned int minutes;
    double seconds;
    convertDecimalDegsToDegMinSec(decimalCoord, degrees, minutes, seconds);
    degrees = abs(degrees);

    //Determine which letter should precede the coordinate
    char dirLetter;
    if (decimalCoord < 0) dirLetter = isLatitude ? 'S' : 'W';
    else dirLetter = isLatitude ? 'N' : 'E';

    //Set up the empty char* in which to format the string
    char value[14];

    //Format the char*
    sprintf(value, "%c%03d.%02u.%03.3f", dirLetter, degrees, minutes, seconds);

    //Convert the formatted char* to a std::string
    string vrcCoord(value, 14);

    return vrcCoord;
}

double GeoUtil::convertVrcToDecimalSingle(const string &vrcCoord) {
    //W077.52.27.771
    string first = vrcCoord.substr(0, 1);
    int sign = first == "N" || first == "E" ? 1 : -1;
    int degrees = (stoi(vrcCoord.substr(1, 3))) * sign;
    unsigned int minutes = stoi(vrcCoord.substr(5, 2));
    double seconds = stod(vrcCoord.substr(8));
    return convertDegMinSecToDecimalDegs(degrees, minutes, seconds);
}

void
GeoUtil::convertVrcToDecimalDegs(const string &vrcLat, const string &vrcLon, double &decimalLat, double &decimalLon) {
    decimalLat = convertVrcToDecimalSingle(vrcLat);
    decimalLon = convertVrcToDecimalSingle(vrcLon);
}

void GeoUtil::convertDecimalDegsToVrc(double decimalLat, double decimalLon, string &vrcLat, string &vrcLon) {
    vrcLat = convertDecimalDegToVrcSingle(decimalLat, true);
    vrcLon = convertDecimalDegToVrcSingle(decimalLon, false);
}

double GeoUtilCalculateDirectBearingAfterTurn(AviationCalcUtil::GeoTools::GeoPoint *aircraft,
                                              AviationCalcUtil::GeoTools::GeoPoint *waypoint, double r,
                                              double curBearing) {
    if (aircraft != NULL && waypoint != NULL) {
        return GeoUtil::calculateDirectBearingAfterTurn(*aircraft, *waypoint, r, curBearing);
    }
    return -1;
}

double GeoUtilCalculateCrossTrackErrorM(GeoPoint *aircraft, GeoPoint *waypoint, double course, double &requiredCourse,
                                        double &alongTrackDistanceM) {
    if (aircraft != NULL && waypoint != NULL) {
        return GeoUtil::calculateCrossTrackErrorM(*aircraft, *waypoint, course, requiredCourse, alongTrackDistanceM);
    }
    return -1;
}

double GeoPointCalculateArcCourseInfo(GeoPoint *aircraft, GeoPoint *arcCenter, double startRadial, double endRadial,
                                      double radiusM, bool clockwise, double &requiredCourse,
                                      double &alongTrackDistanceM) {
    if (aircraft != NULL && arcCenter != NULL){
        return GeoUtil::calculateArcCourseInfo(*aircraft, *arcCenter, startRadial, endRadial, radiusM, clockwise, requiredCourse, alongTrackDistanceM);
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
    if (pos != NULL && wp != NULL) {
        auto nat_int = std::make_unique<GeoPoint>();
        double toReturn = GeoUtil::calculateTurnLeadDistance(*pos, *wp, trueTrack, tas, course, trueWindDir, windSpd,
                                                             radiusOfTurn, nat_int);
        intersection = nat_int.release();
        return toReturn;
    }
    return -1;
}

GeoPoint *GeoUtilFindIntersection(GeoPoint *position, GeoPoint *wp, double trueTrack, double course) {
    if (position != NULL && wp != NULL) {
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
    tuple<double, double> answer = GeoUtil::calculateChordHeadingAndDistance(startHeading, degreesTurned,
                                                                             radiusOfTurnNMi, isRightTurn);
    chordHeading = std::get<0>(answer);
    chordDistance = std::get<1>(answer);
}

double GeoUtilCalculateTurnAmount(double currentHeading, double desiredHeading) {
    return GeoUtil::calculateTurnAmount(currentHeading, desiredHeading);
}

double GeoUtilCalculateDeltaToHeading(double currentHeading, double desiredHeading, bool isRightTurn){
    return GeoUtil::calculateDeltaToHeading(currentHeading, desiredHeading, isRightTurn);
}

double GeoUtilGetEarthRadiusM() {
    return GeoUtil::EARTH_RADIUS_M;
}

double GeoUtilConvertDegMinSecToDecimalDegs(int degrees, unsigned int mins, double secs) {
    return GeoUtil::convertDegMinSecToDecimalDegs(degrees, mins, secs);
}

void GeoUtilConvertDecimalDegsToDegMinSec(double decimalDegs, int &degrees, unsigned int &mins, double &secs) {
    GeoUtil::convertDecimalDegsToDegMinSec(decimalDegs, degrees, mins, secs);
}

void GeoUtilConvertNatsToDecimalDegs(const char *natsLat, const char *natsLon, double &decLat, double &decLon) {
    if (natsLat == NULL || natsLon == NULL) {
        decLat = -1;
        decLon = -1;
        return;
    }
    string natsLatStr(natsLat);
    string natsLonStr(natsLon);
    GeoUtil::convertNatsToDecimalDegs(natsLatStr, natsLonStr, decLat, decLon);
}

void GeoUtilConvertVrcToDecimalDegs(const char *vrcLat, const char *vrcLon, double &decLat, double &decLon) {
    if (vrcLat == NULL || vrcLon == NULL) {
        decLat = -1;
        decLon = -1;
        return;
    }
    string vrcLatStr(vrcLat);
    string vrcLonStr(vrcLon);
    GeoUtil::convertVrcToDecimalDegs(vrcLatStr, vrcLonStr, decLat, decLon);
}

void GeoUtilConvertDecimalDegsToNats(double decimalLat, double decimalLon, const char **natsLat, const char **natsLon) {
    string natsLatStr{};
    string natsLonStr{};
    GeoUtil::convertDecimalDegsToNats(decimalLat, decimalLon, natsLatStr, natsLonStr);
    *natsLat = natsLatStr.c_str();
    *natsLon = natsLonStr.c_str();
}

void GeoUtilConvertDecimalDegsToVrc(double decimalLat, double decimalLon, const char **vrcLat, const char **vrcLon) {
    string vrcLatStr{};
    string vrcLonStr{};
    GeoUtil::convertDecimalDegsToVrc(decimalLat, decimalLon, vrcLatStr, vrcLonStr);
    *vrcLat = vrcLatStr.c_str();
    *vrcLon = vrcLonStr.c_str();
}
