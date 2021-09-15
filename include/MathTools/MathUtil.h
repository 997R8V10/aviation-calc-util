//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_MATHUTIL_H
#define AVIATION_SIM_UTIL_MATHUTIL_H

#include "aviationcalc_exports.h"

namespace AviationCalcUtil::MathTools {
    class AVIATIONCALC_EXPORT MathUtil {
    public:
        // Constants
        constexpr static double CONV_FACTOR_KELVIN_C = 273.15;
        constexpr static double CONV_FACTOR_M_FT = 3.28084;
        constexpr static double CONV_FACTOR_HPA_INHG = 33.86;
        constexpr static double CONV_FACTOR_NMI_M = 1852;
        constexpr static double CONV_FACTOR_MPERS_KTS = 1.943844;

        // Methods
        static double convertDegreesToRadians(double degrees);

        static double convertRadiansToDegrees(double radians);

        static double convertHpaToInhg(double hPa);

        static double convertInhgToHpa(double inHg);

        // Length
        static double convertMetersToFeet(double meters);

        static double convertFeetToMeters(double feet);

        static double convertMetersToNauticalMiles(double meters);

        static double convertNauticalMilesToMeters(double NMi);

        // Temperature
        static double convertKelvinToCelsius(double kelvin);

        static double convertCelsiusToKelvin(double celsius);

        // Velocity
        static double convertMpersToKts(double mpers);

        static double convertKtsToMpers(double kts);

        static long factorial(int n);
    };
}

extern "C"
{
extern AVIATIONCALC_EXPORT double MathUtilGetConvFactorKelvinC();
extern AVIATIONCALC_EXPORT double MathUtilGetConvFactorMFt();
extern AVIATIONCALC_EXPORT double MathUtilGetConvFactorHpaInhg();
extern AVIATIONCALC_EXPORT double MathUtilGetConvFactorNmiM();
extern AVIATIONCALC_EXPORT double MathUtilGetConvFactorMpersKts();
extern AVIATIONCALC_EXPORT double MathUtilConvertDegreesToRadians(double degrees);
extern AVIATIONCALC_EXPORT double MathUtilConvertRadiansToDegrees(double radians);
extern AVIATIONCALC_EXPORT double MathUtilConvertHpaToInhg(double hPa);
extern AVIATIONCALC_EXPORT double MathUtilConvertInhgToHpa(double inHg);
extern AVIATIONCALC_EXPORT double MathUtilConvertMetersToFeet(double meters);
extern AVIATIONCALC_EXPORT double MathUtilConvertFeetToMeters(double feet);
extern AVIATIONCALC_EXPORT double MathUtilConvertMetersToNauticalMiles(double meters);
extern AVIATIONCALC_EXPORT double MathUtilConvertNauticalMilesToMeters(double NMi);
extern AVIATIONCALC_EXPORT double MathUtilConvertKelvinToCelsius(double kelvin);
extern AVIATIONCALC_EXPORT double MathUtilConvertCelsiusToKelvin(double celsius);
extern AVIATIONCALC_EXPORT double MathUtilConvertMpersToKts(double mpers);
extern AVIATIONCALC_EXPORT double MathUtilConvertKtsToMpers(double kts);
extern AVIATIONCALC_EXPORT long MathUtilFactorial(int n);
}

#endif //AVIATION_SIM_UTIL_MATHUTIL_H
