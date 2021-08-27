//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
#define AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H

#include <boost/date_time/gregorian/greg_date.hpp>
#include <memory>

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticModelCoefficients {
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

        void setN(int newN);

        int getM() const;

        void setM(int newM);

        double getG() const;

        void setG(double newG);

        double getH() const;

        void setH(double newH);

        double getGDot() const;

        void setGDot(double newGDot);

        double getHDot() const;

        void setHDot(double newHDot);

        std::unique_ptr<MagneticModelCoefficients> getPointOnDate(double modelEpoch, const boost::gregorian::date &date) const;
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
