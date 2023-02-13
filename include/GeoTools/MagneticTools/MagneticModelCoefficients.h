//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
#define AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H

#include <boost/date_time/gregorian/greg_date.hpp>
#include <memory>
#include "aviationcalc_exports.h"
#include "InteropTools/InteropUtil.h"

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class AVIATIONCALC_EXPORT MagneticModelCoefficients {
    private:
        int n;
        int m;
        double g_nm;
        double h_nm;
        double g_dot_nm;
        double h_dot_nm;

    public:
        MagneticModelCoefficients(const MagneticModelCoefficients &o);

        MagneticModelCoefficients(int n, int m, double g_nm, double h_nm, double g_dot_nm, double h_dot_nm);

        int getN() const;

        int getM() const;

        double getG() const;

        double getH() const;

        double getGDot() const;

        double getHDot() const;

        std::unique_ptr<MagneticModelCoefficients> getPointOnDate(double modelEpoch, const boost::gregorian::date &date) const;
    };
}

using namespace AviationCalcUtil::GeoTools::MagneticTools;

extern "C"{
    extern AVIATIONCALC_EXPORT MagneticModelCoefficients *CopyMagModelCoeffs(MagneticModelCoefficients *o);
    extern AVIATIONCALC_EXPORT MagneticModelCoefficients *CreateMagModelCoeffs(int n, int m, double g_nm, double h_nm, double g_dot_nm, double h_dot_nm);
    extern AVIATIONCALC_EXPORT void DisposeMagModelCoeffs(MagneticModelCoefficients *coeffs);
    extern AVIATIONCALC_EXPORT int MagModelCoeffsGetN(MagneticModelCoefficients *coeffs);
    extern AVIATIONCALC_EXPORT int MagModelCoeffsGetM(MagneticModelCoefficients *coeffs);
    extern AVIATIONCALC_EXPORT double MagModelCoeffsGetG(MagneticModelCoefficients *coeffs);
    extern AVIATIONCALC_EXPORT double MagModelCoeffsGetH(MagneticModelCoefficients *coeffs);
    extern AVIATIONCALC_EXPORT double MagModelCoeffsGetGDot(MagneticModelCoefficients *coeffs);
    extern AVIATIONCALC_EXPORT double MagModelCoeffsGetHDot(MagneticModelCoefficients *coeffs);
    extern AVIATIONCALC_EXPORT MagneticModelCoefficients *MagModelCoeffsGetPointOnDate(MagneticModelCoefficients *coeffs, double modelEpoch, InteropDateStruct dateStruct);
}

#endif //AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
