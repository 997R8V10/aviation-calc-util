//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
#define AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H

#include <boost/date_time/gregorian/greg_date.hpp>
#include <memory>
#include "aviationcalc_exports.h"

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

#endif //AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
