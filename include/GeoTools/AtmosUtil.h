//
// Created by pshivaraman on 2021-09-03.
//

#ifndef AVIATION_CALC_UTIL_ATMOSUTIL_H
#define AVIATION_CALC_UTIL_ATMOSUTIL_H

#include "MathTools/MathUtil.h"

namespace AviationCalcUtil::GeoTools {
    class AtmosUtil {
    public:
        /// Specific Gas Constant for dry air (J/(kg*K))
        AVIATIONCALC_EXPORT static constexpr double R_DRY_AIR = 287.058;

        /// Ratio of specific heat at a constant pressure to heat at a constant volume for air
        AVIATIONCALC_EXPORT static constexpr double SPEC_HEAT_RATIO_AIR = 1.4;

        /// ISA Sea Level Standard Temperature (K)
        AVIATIONCALC_EXPORT static constexpr double ISA_STD_TEMP_K = 288.15;

        /// ISA Sea Level Standard Temperature (C)
        AVIATIONCALC_EXPORT static constexpr double ISA_STD_TEMP_C =
                ISA_STD_TEMP_K - MathTools::MathUtil::CONV_FACTOR_KELVIN_C;

        /// ISA Sea Level Standard Pressure (Pa)
        AVIATIONCALC_EXPORT static constexpr double ISA_STD_PRES_Pa = 101325;

        /// ISA Sea Level Standard Pressure (hPa)
        AVIATIONCALC_EXPORT static constexpr double ISA_STD_PRES_hPa = ISA_STD_PRES_Pa / 100;

        /// ISA Sea Level Air Density (kg/m^3)
        AVIATIONCALC_EXPORT static constexpr double ISA_STD_DENS = ISA_STD_PRES_Pa / (R_DRY_AIR * ISA_STD_TEMP_K);

        /// ISA Standard Lapse Rate (K/m) below 11000m altitude.
        AVIATIONCALC_EXPORT static constexpr double ISA_STD_LAPSE_RATE = 0.0065;

        /// Earth Surface Gravitational Acceleration (m/s^2)
        AVIATIONCALC_EXPORT static constexpr double EARTH_G = 9.80665;

        /// Conversion Factor between hPa and ft of altitude.
        AVIATIONCALC_EXPORT static constexpr double ISA_STD_PRES_DROP_ft_PER_hPa = 30.0;

        /// Calculates the density of dry air at a given temperature and pressure.
        /// \param p Absolute pressure (Pa)
        /// \param T Absolute temperature (K)
        /// \return rho: Dry air density (kg/m^3)
        AVIATIONCALC_EXPORT static double calculateDryAirDensity(double p, double T);

        /// Calculates the impact pressure
        /// \param cas Calibrated airspeed (m/s)
        /// \return qc: Impact pressure (Pa)
        AVIATIONCALC_EXPORT static double calculateImpactPressure(double cas);

        /// Calculates the impact pressure
        /// \param M Mach number
        /// \param p Absolute pressure (Pa)
        /// \return qc: Impact pressure (Pa)
        AVIATIONCALC_EXPORT static double calculateImpactPressure(double M, double p);

        /// Calculates the calibrated air speed
        /// \param qc Impact pressure (Pa)
        /// \return cas: Calibrated air speed (m/s)
        AVIATIONCALC_EXPORT static double calculateCalibratedAirspeed(double qc);

        /// Calculates the mach number
        /// \param qc Impact pressure (Pa)
        /// \param p Absolute pressure (Pa)
        /// \return M: Mach number
        AVIATIONCALC_EXPORT static double calculateMachNumber(double qc, double p);

        /// Calculates the equivalent air speed
        /// \param M Mach number
        /// \param p Absolute pressure (Pa)
        /// \return eas: Equivalent air speed (m/s)
        AVIATIONCALC_EXPORT static double calculateEas(double M, double p);

        /// Calculates the true air speed from mach number
        /// \param M Mach number
        /// \param T Absolute temperature (K)
        /// \return tas: True air speed (m/s)
        AVIATIONCALC_EXPORT static double convertMachToTas(double M, double T);

        /// Calculates the mach number from the true air speed.
        /// \param tas True air speed (m/s)
        /// \param T Absolute temperature (K)
        /// \return M: Mach number
        AVIATIONCALC_EXPORT static double convertTasToMach(double tas, double T);

        /// Calculates the static air pressure at a given altitude
        /// \param h Desired altitude above mean sea level (m)
        /// \param h0 Reference altitude above mean sea level (m)
        /// \param p0 Reference static air pressure (Pa)
        /// \param T Absolute temperature at h (K)
        /// \return p: Static air pressure at h (Pa)
        AVIATIONCALC_EXPORT static double calculatePressureAtAlt(double h, double h0, double p0, double T);

        /// Calculates the absolute temperature at a given altitude
        /// \param h Desired altitude above mean sea level (m)
        /// \param h0 Reference altitude above mean sea level (m)
        /// \param T0 Reference absolute temperature (K)
        /// \return T: Absolute temperature at h (K)
        AVIATIONCALC_EXPORT static double calculateTempAtAlt(double h, double h0, double T0);

        /// Calculates the density altitude
        /// \param p Absolute pressure (Pa)
        /// \param T Absolute temperature (K)
        /// \return Density altitude (m)
        AVIATIONCALC_EXPORT static double calculateDensityAltitude(double p, double T);

        /// Calculates the speed of sound in dry air at a given temperature
        /// \param T Absolute temperature (K)
        /// \return Speed of sound (m/s)
        AVIATIONCALC_EXPORT static double calculateSpeedOfSoundDryAir(double T);

        /// Converts Indicated Air Speed to True Air Speed
        /// \param ias_kts Indicated air speed (kts)
        /// \param refPress_hPa Reference pressure (hPa)
        /// \param alt_ft True altitude (ft)
        /// \param refAlt_ft Reference altitude (ft)
        /// \param refTemp_K Reference temperature (K)
        /// \param mach output Mach number
        /// \return True air speed (kts)
        AVIATIONCALC_EXPORT static double convertIasToTas(double ias_kts, double refPress_hPa, double alt_ft, double refAlt_ft, double refTemp_K, double &mach);

        /// Converts True Air Speed to Indicated Air Speed
        /// \param tas_kts True air speed (kts)
        /// \param refPress_hPa Reference pressure (hPa)
        /// \param alt_ft True altitude (ft)
        /// \param refAlt_ft Reference altitude (ft)
        /// \param refTemp_K Reference temperature (K)
        /// \param mach output Mach number
        /// \return Indicated air speed (kts)
        AVIATIONCALC_EXPORT static double convertTasToIas(double tas_kts, double refPress_hPa, double alt_ft, double refAlt_ft, double refTemp_K, double &mach);

        /// Converts Indicated Altitude to Absolute Altitude
        /// \param alt_ind_ft Indicated altitude (ft)
        /// \param pres_set_hpa Pressure setting (hPa)
        /// \param sfc_pres_hpa Surface pressure (hPa)
        /// \return Absolute altitude (ft)
        AVIATIONCALC_EXPORT static double convertIndicatedToAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa);

        /// Converts Absolute Altitude to Indicated Altitude
        /// \param alt_abs_ft Absolute altitude (ft)
        /// \param pres_set_hpa Pressure setting (hPa)
        /// \param sfc_pres_hpa Surface pressure (hPa)
        /// \return Indicated altitude (ft)
        AVIATIONCALC_EXPORT static double convertAbsoluteToIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa);

        /// Converts Indicated Altitude to Pressure Altitude
        /// \param alt_ind_ft Indicated altitude (ft)
        /// \param pres_set_hpa Pressure setting (hPa)
        /// \return Pressure altitude (ft)
        AVIATIONCALC_EXPORT static double convertIndicatedToPressureAlt(double alt_ind_ft, double pres_set_hpa);

        /// Calculate ISA (International Standard Atmosphere) Temperature at a Pressure Altitude.
        /// \param alt_pres_ft Pressure altitude (ft)
        /// \return ISA Temp (C)
        AVIATIONCALC_EXPORT static double calculateIsaTemp(double alt_pres_ft);
    };
}

extern "C" {
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_R_DRY_AIR();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_SPEC_HEAT_RATIO_AIR();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_ISA_STD_TEMP_K();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_ISA_STD_TEMP_C();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_ISA_STD_PRES_Pa();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_ISA_STD_PRES_hPa();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_ISA_STD_DENS();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_ISA_STD_LAPSE_RATE();
    extern AVIATIONCALC_EXPORT double AtmosUtilGetConst_EARTH_G();
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateDryAirDensity(double p, double T);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateImpactPressure1(double cas);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateImpactPressure2(double M, double p);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateCalibratedAirspeed(double qc);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateMachNumber(double qc, double p);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateEas(double M, double p);
extern AVIATIONCALC_EXPORT double AtmosUtilConvertMachToTas(double M, double T);
extern AVIATIONCALC_EXPORT double AtmosUtilConvertTasToMach(double tas, double T);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculatePressureAtAlt(double h, double h0, double p0, double T);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateTempAtAlt(double h, double h0, double T0);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateDensityAltitude(double p, double T);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateSpeedOfSoundDryAir(double T);
extern AVIATIONCALC_EXPORT double AtmosUtilConvertIasToTas(double ias_kts, double refPress_hPa, double alt_ft, double refAlt_ft, double refTemp_K, double &mach);
extern AVIATIONCALC_EXPORT double AtmosUtilConvertTasToIas(double tas_kts, double refPress_hPa, double alt_ft, double refAlt_ft, double refTemp_K, double &mach);
extern AVIATIONCALC_EXPORT double AtmosUtilConvertIndicatedToAbsoluteAlt(double alt_ind_ft, double pres_set_hpa, double sfc_pres_hpa);
extern AVIATIONCALC_EXPORT double AtmosUtilConvertAbsoluteToIndicatedAlt(double alt_abs_ft, double pres_set_hpa, double sfc_pres_hpa);
extern AVIATIONCALC_EXPORT double AtmosUtilConvertIndicatedToPressureAlt(double alt_ind_ft, double pres_set_hpa);
extern AVIATIONCALC_EXPORT double AtmosUtilCalculateIsaTemp(double alt_pres_ft);
}

#endif //AVIATION_CALC_UTIL_ATMOSUTIL_H
