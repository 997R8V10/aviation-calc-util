//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_GEOUTIL_H
#define AVIATION_SIM_UTIL_GEOUTIL_H

#include "GeoTools/GeoPoint.h"
#include <tuple>
#include <string>
#include <memory>
#include "aviationcalc_exports.h"

using namespace std;

namespace AviationCalcUtil::GeoTools {
    class AVIATIONCALC_EXPORT GeoUtil {

    private:
        /// Convert a single decimal degree to a VRC/Euroscope style coordinate.
        /// \param decimalCoord Decimal degree
        /// \return VRC/Euroscope style coordinate
        static string convertDecimalDegToVrcSingle(double decimalCoord, bool isLatitude);

        /// Convert a single VRC/Euroscope style coordinate to a decimal degree.
        /// \param vrcCoord VRC/Euroscope style coordinate
        /// \return decimal degree
        static double convertVrcToDecimalSingle(const std::string &vrcCoord);

        /// Convert a single NATS style coordinate to a decimal degree.
        /// \param natsCoord NATS style coordinate
        /// \param isLatitude True if the coordinate is latitude, false if it is longitude
        static double convertNatsToDecimalSingle(const std::string &natsCoord, bool isLatitude);

        /// Convert a single NATS style coordinate to a decimal degree.
        /// \param decimalCoord Decimal degree
        /// \param isLatitude True if the coordinate is latitude, false if it is longitude
        static string convertDecimalToNatsSingle(double decimalCoord, bool isLatitude);

    public:
        static constexpr double EARTH_RADIUS_M = 6371e3;

        /// Calculates the direct course to intercept towards a waypoint.
        /// Returns -1 if direct course is not possible to achieve.
        /// \param aircraft Aircraft Position
        /// \param waypoint Waypoint Position
        /// \param r Radius of Turn
        /// \param curBearing Aircraft's Current Bearing
        /// \return Direct bearing to waypoint after turn
        static double calculateDirectBearingAfterTurn(const GeoPoint &aircraft, const GeoPoint &waypoint,
                                                      double r, double curBearing);

        /// Calculates cross track error in meters.
        /// \param aircraft Aircraft's position.
        /// \param waypoint Waypoint's position.
        /// \param course Course to waypoint (degrees).
        /// \param requiredCourse Output's course along great circle path at current position.
        /// \param alongTrackDistanceM Output's distance along great circle path. Negative if station passage has occurred.
        /// \return Cross track error (m). Right: Positive; Left: Negative;
        static double calculateCrossTrackErrorM(const GeoPoint &aircraft, const GeoPoint &waypoint, double course,
                                                double &requiredCourse, double &alongTrackDistanceM);

        /// Calculates required lead in distance for a turn.
        /// \param point Intersection between current course and desired course
        /// \param theta Amount of turn in degrees
        /// \param r Radius of turn
        /// \return Calculates distance prior to point that turn must begin. Units will match the units for r.
        static double calculateTurnLeadDistance(const GeoPoint &point, double theta, double r);

        /// Calculates lead distance for a turn.
        /// \param pos Aircraft's current position.
        /// \param wp Waypoint position.
        /// \param trueTrack Aircraft's current course (degrees true)
        /// \param tas Aircraft's Speed (KTAS)
        /// \param course Course to intercept to waypoint (degrees true)
        /// \param trueWindDir Wind Direction (degrees true)
        /// \param windSpd Wind Speed (kts)
        /// \param radiusOfTurn Output Radius of Turn (nautical miles)
        /// \param intersection Output Intersection Point
        /// \return Lead distance from intersection that turn must begin (nautical miles)
        static double calculateTurnLeadDistance(const GeoPoint &pos, const GeoPoint &wp, double trueTrack, double tas,
                                                double course, double trueWindDir, double windSpd, double &radiusOfTurn,
                                                std::unique_ptr<GeoPoint> &intersection);

        /// Calculates the intersection between the aircraft's current track and a course to/from a waypoint.
        /// \param position Aircraft Position
        /// \param wp Waypoint
        /// \param course Course To/From Waypoint (degrees).
        /// \return The intersection, should one exist; otherwise null
        static std::unique_ptr<GeoPoint> findIntersection(const GeoPoint &position, const GeoPoint &wp,
                                                          double trueTrack, double course);

        /// Normalizes Longitude between -180 and +180 degrees
        /// \param lon Input Longitude (degrees)
        /// \return Normalized Longitude (degrees)
        static double normalizeLongitude(double lon);

        /// Normalizes Heading between 0 and 360 degrees
        /// \param hdg Input Heading (degrees)
        /// \return Normalized Heading (degrees)
        static double normalizeHeading(double hdg);

        /// Calculates maximum bank angle for a given rate of turn.
        /// \param groundSpeed Ground speed (kts)
        /// \param bankLimit Bank limit (degrees)
        /// \param turnRate Rate of turn (degrees/s)
        /// \return Bank angle (degrees)
        static double calculateMaxBankAngle(double groundSpeed, double bankLimit, double turnRate);

        /// Calculates radius of turn at a certain bank angle.
        /// \param bankAngle Bank angle (degrees)
        /// \param groundSpeed Ground speed (kts)
        /// \return Radius of turn (nautical miles)
        static double calculateRadiusOfTurn(double bankAngle, double groundSpeed);

        /// Calculates bank angle at a certain radius of turn.
        /// \param radiusOfTurn Radius of turn (nautical miles)
        /// \param groundSpeed Ground speed (kts)
        /// \return Bank angle (degrees)
        static double calculateBankAngle(double radiusOfTurn, double groundSpeed);

        /// Calculates the radius of turn required for a constant radius turn with wind.
        /// \param startBearing Start bearing (degrees)
        /// \param turnAmount Amount to turn (degrees)
        /// \param windBearing Bearing that wind is coming from (degrees)
        /// \param windSpeed Wind speed (kts)
        /// \param tas True air speed (kts)
        /// \return Radius of turn (nautical miles)
        static double calculateConstantRadiusTurn(double startBearing, double turnAmount, double windBearing,
                                                  double windSpeed, double tas);

        /// Gets the headwind/tailwind component.
        /// \param windSpeed Wind speed (kts)
        /// \param windBearing Bearing that wind is coming from (degrees)
        /// \param bearing Bearing that aircraft is headed in (degrees)
        /// \return Headwind Component (negative for tailwind) (kts)
        static double getHeadwindComponent(double windSpeed, double windBearing, double bearing);

        /// Calculate distance travelled in nautical miles.
        /// \param groundSpeedKts Ground speed (kts)
        /// \param timeMs Time (ms)
        /// \return Distance travelled (nautical miles)
        static double calculateDistanceTravelledNMi(double groundSpeedKts, double timeMs);

        /// Calculate amount of degrees turned.
        /// \param distTravelledNMi Distance travelled (nautical miles)
        /// \param radiusOfTurnNMi Radius of turn (nautical miles)
        /// \return Degrees turned (degrees)
        static double calculateDegreesTurned(double distTravelledNMi, double radiusOfTurnNMi);

        /// Calculate roll out heading.
        /// \param startHeading Start heading (degrees)
        /// \param degreesTurned Amount of turn (degrees)
        /// \param isRightTurn Is the turn to the right?
        /// \return Roll out heading (degrees)
        static double calculateEndHeading(double startHeading, double degreesTurned, bool isRightTurn);

        /// Calculate direct heading and distance along a circle (chord line)
        /// \param startHeading Start heading (degrees)
        /// \param degreesTurned Amount of turn (degrees)
        /// \param radiusOfTurnNMi Radius of turn (nautical miles)
        /// \param isRightTurn Is the turn to the right?
        /// \return Chord heading (degrees), Chord distance (nautical miles)
        static tuple<double, double> calculateChordHeadingAndDistance(double startHeading, double degreesTurned,
                                                                      double radiusOfTurnNMi, bool isRightTurn);

        /// Calculate amount of turn between two headings (0-360)
        /// \param currentHeading Current heading (degrees)
        /// \param desiredHeading Desired heading (degrees)
        /// \return Amount of turn (degrees). Negative is left.
        static double calculateTurnAmount(double currentHeading, double desiredHeading);

        /// Convert from degrees.minutes.seconds to decimal degrees.
        /// \param degrees Degrees (signed integer)
        /// \param mins Minutes (unsigned integer)
        /// \param secs Seconds (double)
        /// \return Decimal degrees
        static double convertDegMinSecToDecimalDegs(int degrees, unsigned int mins, double secs);

        /// Convert from decimal degrees to degrees.minutes.seconds.
        /// \param decimalDegs Decimal degrees
        /// \param degrees Output degrees (signed integer)
        /// \param mins Output minutes (unsigned integer)
        /// \param secs Output seconds (double)
        static void convertDecimalDegsToDegMinSec(double decimalDegs, int &degrees, unsigned int &mins, double &secs);

        /// Convert from NATS style coordinates to decimal degrees.
        /// \param natsLat NATS style latitude
        /// \param natsLon NATS style longitude
        /// \param decimalLat Decimal latitude out variable
        /// \param decimalLon Decimal longitude out variable
        static void convertNatsToDecimalDegs(const std::string &natsLat, const std::string &natsLon, double& decimalLat, double& decimalLon);

        /// Convert from decimal degrees to NATS style coordinates.
        /// \param decimalLat Decimal latitude
        /// \param decimalLon Decimal longitude
        /// \param natsLat Out NATS latitude
        /// \param natsLon Out NATS longitude
        static void convertDecimalDegsToNats(double decimalLat, double decimalLon, string& natsLat, string& natsLon);

        /// Convert from VRC/Euroscope style coordinates to decimal degrees.
        /// \param vrcLat VRC/Euroscope style latitude
        /// \param vrcLon VRC/Euroscope style longitude
        /// \param decimalLat Out decimal latitude
        /// \param decimalLon Out decimal longitude
        static void convertVrcToDecimalDegs(const std::string &vrcLat, const std::string &vrcLon, double& decimalLat, double& decimalLon);

        /// Convert from decimal degrees to VRC/Euroscope style coordinates.
        /// \param decimalLat Decimal latitude
        /// \param decimalLon Decimal longitude
        /// \param vrcLat Out VRC/Euroscope style latitude
        /// \param vrcLon Out VRC/Euroscope style longitude
        static void convertDecimalDegsToVrc(double decimalLat, double decimalLon, string& vrcLat, string& vrcLon);
    };
}

/**using namespace AviationCalcUtil::GeoTools;

extern "C"
{
extern AVIATIONCALC_EXPORT double
GeoUtilCalculateDirectBearingAfterTurn(GeoPoint *aircraft, GeoPoint *waypoint, double r, double curBearing);
extern AVIATIONCALC_EXPORT double
GeoUtilCalculateCrossTrackErrorM(GeoPoint *aircraft, GeoPoint *waypoint, double course,
                                 double &requiredCourse, double &alongTrackDistanceM);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateTurnLeadInDistance(GeoPoint *ptr, double theta, double r);
extern AVIATIONCALC_EXPORT double
GeoUtilCalculateTurnLeadDistance(GeoPoint *pos, GeoPoint *wp, double trueTrack, double tas,
                                 double course, double trueWindDir, double windSpd, double &radiusOfTurn,
                                 GeoPoint *&intersection);
extern AVIATIONCALC_EXPORT GeoPoint *GeoUtilFindIntersection(GeoPoint *position, GeoPoint *wp,
                                                             double trueTrack, double course);
extern AVIATIONCALC_EXPORT double GeoUtilNormalizeLongitude(double lon);
extern AVIATIONCALC_EXPORT double GeoUtilNormalizeHeading(double hdg);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateMaxBankAngle(double groundSpeed, double bankLimit, double turnRate);

extern AVIATIONCALC_EXPORT double GeoUtilCalculateRadiusOfTurn(double bankAngle, double groundSpeed);

extern AVIATIONCALC_EXPORT double GeoUtilCalculateBankAngle(double radiusOfTurn, double groundSpeed);

extern AVIATIONCALC_EXPORT double GeoUtilCalculateConstantRadiusTurn(double startBearing, double turnAmount, double windBearing, double windSpeed, double tas);
extern AVIATIONCALC_EXPORT double GeoUtilGetHeadwindComponent(double windSpeed, double windBearing, double bearing);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateDistanceTravelledNMi(double groundSpeedKts, double timeMs);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateDegreesTurned(double distTravelledNMi, double radiusOfTurnNMi);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateEndHeading(double startHeading, double degreesTurned, bool isRightTurn);
extern AVIATIONCALC_EXPORT void GeoUtilCalculateChordHeadingAndDistance(double startHeading, double degreesTurned, double radiusOfTurnNMi, bool isRightTurn, double &chordHeading, double &chordDistance);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateTurnAmount(double currentHeading, double desiredHeading);
extern AVIATIONCALC_EXPORT double GeoUtilGetEarthRadiusM();
extern AVIATIONCALC_EXPORT double GeoUtilConvertDegMinSecToDecimalDegs(int degrees, unsigned int mins, double secs);
extern AVIATIONCALC_EXPORT void GeoUtilConvertDecimalDegsToDegMinSec(double decimalDegs, int &degrees, unsigned int &mins, double &secs);
extern AVIATIONCALC_EXPORT void GeoUtilConvertNatsToDecimalDegs(const char *natsLat, const char *natsLon, double &decLat, double &decLon);
extern AVIATIONCALC_EXPORT void GeoUtilConvertDecimalDegsToNats(double decimalLat, double decimalLon, const char **natsLat, const char **natsLon);
extern AVIATIONCALC_EXPORT void GeoUtilConvertVrcToDecimalDegs(const char *vrcLat, const char *vrcLon, double &decLat, double &decLon);
extern AVIATIONCALC_EXPORT void GeoUtilConvertDecimalDegsToVrc(double decimalLat, double decimalLon, const char **vrcLat, const char **vrcLon);
}*/


#endif //AVIATION_SIM_UTIL_GEOUTIL_H
