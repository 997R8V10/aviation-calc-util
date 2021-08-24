//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_GEOUTIL_H
#define AVIATION_SIM_UTIL_GEOUTIL_H

#include "GeoPoint.h"
#include <tuple>

using namespace std;

namespace AviationCalcUtil::GeoTools {
    class GeoUtil {
    public:
        static const double EARTH_RADIUS_M;
        static const double STD_PRES_HPA;
        static const double STD_TEMP_C;
        static const double STD_LAPSE_RATE;
        static const double STD_PRES_DROP;

        /// Calculates the direct course to intercept towards a waypoint.
        /// Returns -1 if direct course is not possible to achieve.
        /// \param aircraft Aircraft Position
        /// \param waypoint Waypoint Position
        /// \param r Radius of Turn
        /// \param curBearing Aircraft's Current Bearing
        /// \return Direct bearing to waypoint after turn
        static double calculateDirectBearingAfterTurn(GeoPoint *aircraft, GeoPoint *waypoint,
                                                      double r, double curBearing);

        /// Calculates cross track error in meters.
        /// \param aircraft Aircraft's position.
        /// \param waypoint Waypoint's position.
        /// \param course Course to waypoint (degrees).
        /// \param requiredCourse Output's course along great circle path at current position.
        /// \param alongTrackDistanceM Output's distance along great circle path. Negative if station passage has occurred.
        /// \return Cross track error (m). Right: Positive; Left: Negative;
        static double
        calculateCrossTrackErrorM(GeoPoint *aircraft, GeoPoint *waypoint, double course, double &requiredCourse,
                                  double &alongTrackDistanceM);

        /// Calculates required lead in distance for a turn.
        /// \param point Intersection between current course and desired course
        /// \param theta Amount of turn in degrees
        /// \param r Radius of turn
        /// \return Calculates distance prior to point that turn must begin. Units will match the units for r.
        static double calculateTurnLeadDistance(GeoPoint *point, double theta, double r);

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
        static double calculateTurnLeadDistance(GeoPoint *pos, GeoPoint *wp, double trueTrack, double tas,
                                                double course, double trueWindDir, double windSpd, double &radiusOfTurn,
                                                GeoPoint &intersection);

        /// Calculates the intersection between the aircraft's current track and a course to/from a waypoint.
        /// \param position Aircraft Position
        /// <param name="wp"><c>GeoPoint</c> Waypoint</param>
        /// <param name="course"><c>double</c> Course To/From Waypoint (degrees).</param>
        /// <returns><c>GeoPoint</c> The intersection, should one exist; otherwise <c>null</c></returns>
        static GeoPoint *findIntersection(GeoPoint *position, GeoPoint *wp, double trueTrack, double course);

        /// <summary>
        /// Normalizes Longitude between -180 and +180 degrees
        /// </summary>
        /// <param name="lon">Input Longitude (degrees)</param>
        /// <returns>Normalized Longitude (degrees)</returns>
        static double normalizeLongitude(double lon);

        /// <summary>
        /// Normalizes Heading between 0 and 360 degrees
        /// </summary>
        /// <param name="hdg">Input Heading (degrees)</param>
        /// <returns>Normalized Heading (degrees)</returns>
        static double normalizeHeading(double hdg);

        static double calculateMaxBankAngle(double groundSpeed, double bankLimit, double turnRate);

        static double calculateRadiusOfTurn(double bankAngle, double groundSpeed);

        static double calculateBankAngle(double radiusOfTurn, double groundSpeed);

        static double
        calculateConstantRadiusTurn(double startBearing, double turnAmount, double windBearing, double windSpeed,
                                    double tas);

        static double getHeadwindComponent(double windSpeed, double windBearing, double bearing);

        static double calculateDistanceTravelledNMi(double groundSpeedKts, double timeMs);

        static double calculateDegreesTurned(double distTravelledNMi, double radiusOfTurnNMi);

        static double calculateEndHeading(double startHeading, double degreesTurned, bool isRightTurn);

        static tuple<double, double>
        calculateChordHeadingAndDistance(double startHeading, double degreesTurned, double radiusOfTurnNMi,
                                         bool isRightTurn);

        static double calculateAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa);

        static double calculateIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa);

        static double calculatePressureAlt(double alt_ind_ft, double pres_set_hpa);

        static double calculateIsaTemp(double alt_pres_ft);

        static double calculateDensityAlt(double alt_pres_ft, double sat);

        static double calculateTAS(double ias, double pres_set_hpa, double alt_ind_ft, double sat);

        static double calculateTurnAmount(double currentHeading, double desiredHeading);
    };
}

#endif //AVIATION_SIM_UTIL_GEOUTIL_H
