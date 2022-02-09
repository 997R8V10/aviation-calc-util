//
// Created by pshivaraman on 2021-09-03.
//

#include "GeoTools/AtmosUtil.h"
#include "MathTools/MathUtil.h"

#define _USE_MATH_DEFINES

#include <cmath>

using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::MathTools;
using namespace std;

// https://en.wikipedia.org/wiki/Density_of_air
double AtmosUtil::calculateDryAirDensity(double p, double T) {
    return p / (R_DRY_AIR * T);
}

// https://www.omnicalculator.com/physics/air-pressure-at-altitude
double AtmosUtil::calculatePressureAtAlt(double h, double h0, double p0, double T) {
    double ePow = (-EARTH_G * (h - h0)) / (R_DRY_AIR * T);
    return p0 * pow(M_E, ePow);
}

double AtmosUtil::calculateTempAtAlt(double h, double h0, double T0) {
    return T0 - ((h - h0) * ISA_STD_LAPSE_RATE);
}

// https://www.omnicalculator.com/physics/air-pressure-at-altitude
double AtmosUtil::calculateDensityAltitude(double p, double T) {
    double presRatio = p / ISA_STD_PRES_Pa;
    double tempRatio = T / ISA_STD_TEMP_K;
    double exp = pow((EARTH_G / (ISA_STD_LAPSE_RATE * R_DRY_AIR)) - 1, -1);
    return (ISA_STD_TEMP_K / ISA_STD_LAPSE_RATE) * (1 - pow(presRatio / tempRatio, exp));
}

// https://en.wikipedia.org/wiki/Calibrated_airspeed
double AtmosUtil::calculateImpactPressure(double cas) {
    double a0 = calculateSpeedOfSoundDryAir(ISA_STD_TEMP_K);

    double coeff1 = 2 / (SPEC_HEAT_RATIO_AIR - 1);
    double coeff2 = SPEC_HEAT_RATIO_AIR / (SPEC_HEAT_RATIO_AIR - 1);

    double x1 = (pow(cas / a0, 2) + coeff1) / coeff1;
    double x2 = pow(x1, coeff2) - 1;

    return ISA_STD_PRES_Pa * x2;
}

// https://en.wikipedia.org/wiki/Calibrated_airspeed
double AtmosUtil::calculateCalibratedAirspeed(double qc) {
    double a0 = calculateSpeedOfSoundDryAir(ISA_STD_TEMP_K);

    double coeff1 = 2 / (SPEC_HEAT_RATIO_AIR - 1);
    double coeff2 = (SPEC_HEAT_RATIO_AIR - 1) / SPEC_HEAT_RATIO_AIR;

    double x1 = pow((qc / ISA_STD_PRES_Pa) + 1, coeff2);
    double x2 = sqrt(coeff1 * (x1 - 1));

    return a0 * x2;
}

// https://en.wikipedia.org/wiki/Mach_number
double AtmosUtil::calculateMachNumber(double qc, double p) {
    double x1 = 2 / (SPEC_HEAT_RATIO_AIR - 1);
    double x2exp = (SPEC_HEAT_RATIO_AIR - 1) / SPEC_HEAT_RATIO_AIR;
    double x2 = (qc / p) + 1;

    return sqrt(x1 * (pow(x2, x2exp) - 1));
}

// https://en.wikipedia.org/wiki/Equivalent_airspeed
double AtmosUtil::calculateEas(double M, double p) {
    double a0 = calculateSpeedOfSoundDryAir(ISA_STD_TEMP_K);

    return a0 * M * sqrt(p / ISA_STD_PRES_Pa);
}

// https://en.wikipedia.org/wiki/Mach_number
double AtmosUtil::calculateSpeedOfSoundDryAir(double T) {
    return sqrt(SPEC_HEAT_RATIO_AIR * R_DRY_AIR * T);
}

// https://en.wikipedia.org/wiki/True_airspeed
double AtmosUtil::convertMachToTas(double M, double T) {
    double a0 = calculateSpeedOfSoundDryAir(ISA_STD_TEMP_K);

    return a0 * M * sqrt(T / ISA_STD_TEMP_K);
}

// https://en.wikipedia.org/wiki/Impact_pressure
double AtmosUtil::calculateImpactPressure(double M, double p) {
    double x1 = (SPEC_HEAT_RATIO_AIR - 1) / 2;
    double x2exp = SPEC_HEAT_RATIO_AIR / (SPEC_HEAT_RATIO_AIR - 1);
    double x2 = (1 + x1 * pow(M, 2));

    return p * (pow(x2, x2exp) - 1);
}

// https://en.wikipedia.org/wiki/True_airspeed
double AtmosUtil::convertTasToMach(double tas, double T) {
    // Prevent div by 0
    if (T < 0) {
        T = 1;
    }

    double a0 = calculateSpeedOfSoundDryAir(ISA_STD_TEMP_K);

    return tas / (a0 * sqrt(T / ISA_STD_TEMP_K));
}

double AtmosUtil::convertIasToTas(double ias_kts, double refPress_hPa, double alt_ft, double refAlt_ft,
                                  double refTemp_K, double &mach) {
    // Convert inputs
    double cas = MathUtil::convertKtsToMpers(ias_kts);
    double p0 = refPress_hPa * 100;
    double h = MathUtil::convertFeetToMeters(alt_ft);
    double h0 = MathUtil::convertFeetToMeters(refAlt_ft);
    double T0 = refTemp_K;

    // Calculate pressure and temp at height
    double T = calculateTempAtAlt(h, h0, T0);
    double p = calculatePressureAtAlt(h, h0, p0, T);

    // Calculate impact (dynamic) pressure
    double qc = AtmosUtil::calculateImpactPressure(cas);

    // Calculate mach number
    mach = AtmosUtil::calculateMachNumber(qc, p);

    // Calculate tas
    double tas = AtmosUtil::convertMachToTas(mach, T);

    return MathUtil::convertMpersToKts(tas);
}

double AtmosUtil::convertTasToIas(double tas_kts, double refPress_hPa, double alt_ft, double refAlt_ft,
                                  double refTemp_K, double &mach) {
    // Convert inputs
    double tas = MathUtil::convertKtsToMpers(tas_kts);
    double p0 = refPress_hPa * 100;
    double h = MathUtil::convertFeetToMeters(alt_ft);
    double h0 = MathUtil::convertFeetToMeters(refAlt_ft);
    double T0 = refTemp_K;

    // Calculate pressure and temp at height
    double T = calculateTempAtAlt(h, h0, T0);
    double p = calculatePressureAtAlt(h, h0, p0, T);

    // Calculate mach number
    mach = AtmosUtil::convertTasToMach(tas, T);

    // Calculate impact (dynamic) pressure
    double qc = AtmosUtil::calculateImpactPressure(mach, p);

    // Calculate tas
    double cas = AtmosUtil::calculateCalibratedAirspeed(qc);

    return MathUtil::convertMpersToKts(cas);
}


double AtmosUtil::convertIndicatedToAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa) {
    double pressDiff = pres_set_hpa - sfc_pres_hpa;
    return alt_ind_ft - (ISA_STD_PRES_DROP_ft_PER_hPa * pressDiff);
}

double AtmosUtil::convertAbsoluteToIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa) {
    double pressDiff = pres_set_hpa - sfc_pres_hpa;
    return alt_abs_ft + (ISA_STD_PRES_DROP_ft_PER_hPa * pressDiff);
}

double AtmosUtil::convertIndicatedToPressureAlt(double alt_ind_ft, double pres_set_hpa) {
    double pressDiff = pres_set_hpa - ISA_STD_PRES_hPa;
    return alt_ind_ft - (ISA_STD_PRES_DROP_ft_PER_hPa * pressDiff);
}

double AtmosUtil::calculateIsaTemp(double alt_pres_ft) {
    if (alt_pres_ft >= 36000) {
        return -56.5;
    }

    return ISA_STD_TEMP_C - (alt_pres_ft * ISA_STD_PRES_DROP_ft_PER_hPa);
}

double AtmosUtilCalculateDryAirDensity(double p, double T) {
    return AtmosUtil::calculateDryAirDensity(p, T);
}

double AtmosUtilCalculateImpactPressure1(double cas) {
    return AtmosUtil::calculateImpactPressure(cas);
}

double AtmosUtilCalculateImpactPressure2(double M, double p) {
    return AtmosUtil::calculateImpactPressure(M, p);
}

double AtmosUtilCalculateCalibratedAirspeed(double qc) {
    return AtmosUtil::calculateCalibratedAirspeed(qc);
}

double AtmosUtilCalculateMachNumber(double qc, double p) {
    return AtmosUtil::calculateMachNumber(qc, p);
}

double AtmosUtilCalculateEas(double M, double p) {
    return AtmosUtil::calculateEas(M, p);
}

double AtmosUtilConvertMachToTas(double M, double T) {
    return AtmosUtil::convertMachToTas(M, T);

}

double AtmosUtilConvertTasToMach(double tas, double T) {
    return AtmosUtil::convertTasToMach(tas, T);
}

double AtmosUtilCalculatePressureAtAlt(double h, double h0, double p0, double T) {
    return AtmosUtil::calculatePressureAtAlt(h, h0, p0, T);
}

double AtmosUtilCalculateTempAtAlt(double h, double h0, double T0) {
    return AtmosUtil::calculateTempAtAlt(h, h0, T0);
}

double AtmosUtilCalculateDensityAltitude(double p, double T) {
    return AtmosUtil::calculateDensityAltitude(p, T);
}

double AtmosUtilCalculateSpeedOfSoundDryAir(double T) {
    return AtmosUtil::calculateSpeedOfSoundDryAir(T);
}

double AtmosUtilConvertIasToTas(double ias_kts, double refPress_hPa, double alt_ft, double refAlt_ft, double refTemp_K,
                                double &mach) {
    return AtmosUtil::convertIasToTas(ias_kts, refPress_hPa, alt_ft, refAlt_ft, refTemp_K, mach);
}

double AtmosUtilConvertTasToIas(double tas_kts, double refPress_hPa, double alt_ft, double refAlt_ft, double refTemp_K,
                                double &mach) {
    return AtmosUtil::convertTasToIas(tas_kts, refPress_hPa, alt_ft, refAlt_ft, refTemp_K, mach);
}

double AtmosUtilConvertIndicatedToAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa){
    return AtmosUtil::convertIndicatedToAbsoluteAlt(alt_ind_ft, pres_set_hpa, sfc_pres_hpa);
}

double AtmosUtilConvertAbsoluteToIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa){
    return AtmosUtil::convertAbsoluteToIndicatedAlt(alt_abs_ft, pres_set_hpa, sfc_pres_hpa);
}

double AtmosUtilConvertIndicatedToPressureAlt(double alt_ind_ft, double pres_set_hpa){
    return AtmosUtil::convertIndicatedToPressureAlt(alt_ind_ft, pres_set_hpa);
}

double AtmosUtilCalculateIsaTemp(double alt_pres_ft){
    return AtmosUtil::calculateIsaTemp(alt_pres_ft);
}

double AtmosUtilGetConst_R_DRY_AIR() {
    return AtmosUtil::R_DRY_AIR;
}

double AtmosUtilGetConst_SPEC_HEAT_RATIO_AIR() {
    return AtmosUtil::SPEC_HEAT_RATIO_AIR;
}

double AtmosUtilGetConst_ISA_STD_TEMP_K() {
    return AtmosUtil::ISA_STD_TEMP_K;
}

double AtmosUtilGetConst_ISA_STD_TEMP_C() {
    return AtmosUtil::ISA_STD_TEMP_C;
}

double AtmosUtilGetConst_ISA_STD_PRES_Pa() {
    return AtmosUtil::ISA_STD_PRES_Pa;
}

double AtmosUtilGetConst_ISA_STD_PRES_hPa() {
    return AtmosUtil::ISA_STD_PRES_hPa;
}

double AtmosUtilGetConst_ISA_STD_DENS() {
    return AtmosUtil::ISA_STD_DENS;
}

double AtmosUtilGetConst_ISA_STD_LAPSE_RATE() {
    return AtmosUtil::ISA_STD_LAPSE_RATE;
}

double AtmosUtilGetConst_ISA_STD_PRES_DROP_ft_PER_hPa() {
    return AtmosUtil::ISA_STD_PRES_DROP_ft_PER_hPa;
}

double AtmosUtilGetConst_EARTH_G() {
    return AtmosUtil::EARTH_G;
}
