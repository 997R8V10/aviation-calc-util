//
// Created by pshivaraman on 2021-08-19.
//

#include "MathTools/MathUtil.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace AviationCalcUtil::MathTools;

double MathUtil::convertDegreesToRadians(double degrees) {
    return M_PI * degrees / 180;
}

double MathUtil::convertRadiansToDegrees(double radians) {
    return radians * 180 / M_PI;
}

double MathUtil::convertMetersToFeet(double meters) {
    return meters * CONV_FACTOR_M_FT;
}

double MathUtil::convertFeetToMeters(double feet) {
    return feet / CONV_FACTOR_M_FT;
}

double MathUtil::convertHpaToInhg(double hPa) {
    return hPa * CONV_FACTOR_M_FT;
}

double MathUtil::convertInhgToHpa(double inHg) {
    return inHg / CONV_FACTOR_M_FT;
}

double MathUtil::convertMetersToNauticalMiles(double meters) {
    return meters / CONV_FACTOR_NMI_M;
}

double MathUtil::convertNauticalMilesToMeters(double NMi) {
    return NMi * CONV_FACTOR_NMI_M;
}

long MathUtil::factorial(int n) {
    long result = 1;

    if (n <= 1){
        return 1;
    }

    for (int i = 2; i <= n; i++){
        result *= i;
    }

    return result;
}

double MathUtil::convertKelvinToCelsius(double kelvin) {
    return kelvin - CONV_FACTOR_KELVIN_C;
}

double MathUtil::convertCelsiusToKelvin(double celsius) {
    return celsius + CONV_FACTOR_KELVIN_C;
}

double MathUtil::convertMpersToKts(double mpers) {
    return mpers * CONV_FACTOR_MPERS_KTS;
}

double MathUtil::convertKtsToMpers(double kts) {
    return kts / CONV_FACTOR_MPERS_KTS;
}
