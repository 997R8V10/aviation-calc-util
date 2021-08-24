//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
#define AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H

#include <boost/date_time/gregorian/greg_date.hpp>

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
        MagneticModelCoefficients(int n, int m, double g_nm, double h_nm, double g_dot_nm, double h_dot_nm);

        int getN();

        void setN(int newN);

        int getM();

        void setM(int newM);

        double getG();

        void setG(double newG);

        double getH();

        void setH(double newH);

        double getGDot();

        void setGDot(double newGDot);

        double getHDot();

        void setHDot(double newHDot);

        MagneticModelCoefficients *getCopy();

        MagneticModelCoefficients *getPointOnDate(double modelEpoch, boost::gregorian::date date);
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICMODELCOEFFICIENTS_H
