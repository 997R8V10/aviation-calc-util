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

        // Methods
        static double convertDegreesToRadians(double degrees);

        static double convertRadiansToDegrees(double radians);

        static double convertMetersToFeet(double meters);

        static double convertFeetToMeters(double feet);

        static double convertHpaToInhg(double hPa);

        static double convertInhgToHpa(double inHg);

        static double convertMetersToNauticalMiles(double meters);

        static double convertNauticalMilesToMeters(double NMi);

        static long factorial(int n);
    };
}


#endif //AVIATION_SIM_UTIL_MATHUTIL_H
