//
// Created by pshivaraman on 2021-08-19.
//

#include "MathUtil.h"
#include "../include/MathUtil.h"


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
