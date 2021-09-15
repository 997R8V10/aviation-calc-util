//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_GEOUTIL_H
#define AVIATION_SIM_UTIL_GEOUTIL_H

#include "GeoTools/GeoPoint.h"
#include <tuple>
#include <memory>
#include "aviationcalc_exports.h"

using namespace std;

namespace AviationCalcUtil::GeoTools {
    class AVIATIONCALC_EXPORT GeoUtil {
    public:
        static constexpr double EARTH_RADIUS_M = 6371e3;
        static constexpr double STD_PRES_HPA = 1013.25;
        static constexpr double STD_TEMP_C = 15;
        static constexpr double STD_LAPSE_RATE = 2.0 / 1000.0;
        static constexpr double STD_PRES_DROP = 30.0;


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
        /// <param name="wp"><c>GeoPoint</c> Waypoint</param>
        /// <param name="course"><c>double</c> Course To/From Waypoint (degrees).</param>
        /// <returns><c>GeoPoint</c> The intersection, should one exist; otherwise <c>null</c></returns>
        static std::unique_ptr<GeoPoint> findIntersection(const GeoPoint &position, const GeoPoint &wp,
                                                          double trueTrack, double course);

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

        /// Calculates maximum bank angle
        /// \param groundSpeed
        /// \param bankLimit
        /// \param turnRate
        /// \return
        static double calculateMaxBankAngle(double groundSpeed, double bankLimit, double turnRate);

        static double calculateRadiusOfTurn(double bankAngle, double groundSpeed);

        static double calculateBankAngle(double radiusOfTurn, double groundSpeed);

        static double calculateConstantRadiusTurn(double startBearing, double turnAmount, double windBearing,
                                                  double windSpeed, double tas);

        static double getHeadwindComponent(double windSpeed, double windBearing, double bearing);

        static double calculateDistanceTravelledNMi(double groundSpeedKts, double timeMs);

        static double calculateDegreesTurned(double distTravelledNMi, double radiusOfTurnNMi);

        static double calculateEndHeading(double startHeading, double degreesTurned, bool isRightTurn);

        static tuple<double, double> calculateChordHeadingAndDistance(double startHeading, double degreesTurned,
                                                                      double radiusOfTurnNMi, bool isRightTurn);


        static double convertIndicatedToAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa);

        static double convertAbsoluteToIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa);

        static double convertIndicatedToPressureAlt(double alt_ind_ft, double pres_set_hpa);

        static double calculateIsaTemp(double alt_pres_ft);

        [[deprecated("Inaccurate. Use function in AtmosUtil.")]]
        static double convertPressureToDensityAlt(double alt_pres_ft, double sat);

        [[deprecated("Inaccurate. Use function in AtmosUtil.")]]
        static double convertIasToTas(double ias, double pres_set_hpa, double alt_ind_ft, double sat);

        [[deprecated("Inaccurate. Use function in AtmosUtil.")]]
        static double convertTasToIas(double tas, double pres_set_hpa, double alt_ind_ft, double sat);

        [[deprecated("Inaccurate. Use function in AtmosUtil.")]]
        static double convertIasToTas(double ias, double alt_dens_ft);

        [[deprecated("Inaccurate. Use function in AtmosUtil.")]]
        static double convertTasToIas(double tas, double alt_dens_ft);

        static double calculateTurnAmount(double currentHeading, double desiredHeading);


    };
}

using namespace AviationCalcUtil::GeoTools;

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
extern AVIATIONCALC_EXPORT double GeoUtilConvertIndicatedToAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa);
extern AVIATIONCALC_EXPORT double GeoUtilConvertAbsoluteToIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa);
extern AVIATIONCALC_EXPORT double GeoUtilConvertIndicatedToPressureAlt(double alt_ind_ft, double pres_set_hpa);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateIsaTemp(double alt_pres_ft);
extern AVIATIONCALC_EXPORT double GeoUtilConvertPressureToDensityAlt(double alt_pres_ft, double sat);
extern AVIATIONCALC_EXPORT double GeoUtilConvertIasToTas(double ias, double pres_set_hpa, double alt_ind_ft, double sat);
extern AVIATIONCALC_EXPORT double GeoUtilConvertTasToIas(double tas, double pres_set_hpa, double alt_ind_ft, double sat);
extern AVIATIONCALC_EXPORT double GeoUtilConvertIasToTasFromDensityAltitude(double ias, double alt_dens_ft);
extern AVIATIONCALC_EXPORT double GeoUtilConvertTasToIasDensityAltitude(double tas, double alt_dens_ft);
extern AVIATIONCALC_EXPORT double GeoUtilCalculateTurnAmount(double currentHeading, double desiredHeading);
extern AVIATIONCALC_EXPORT double GeoUtilGetEarthRadiusM();
extern AVIATIONCALC_EXPORT double GeoUtilGetStdPresHpa();
extern AVIATIONCALC_EXPORT double GeoUtilGetStdTempC();
extern AVIATIONCALC_EXPORT double GeoUtilGetStdLapseRate();
extern AVIATIONCALC_EXPORT double GeoUtilGetStdPresDrop();
}


#endif //AVIATION_SIM_UTIL_GEOUTIL_H
