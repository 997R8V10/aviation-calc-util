//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "GeoTools/MagneticTools/MagneticModelCoefficients.h"

using namespace AviationCalcUtil::GeoTools::MagneticTools;

MagneticModelCoefficients::MagneticModelCoefficients(int n, int m, double g_nm, double h_nm, double g_dot_nm,
                                                     double h_dot_nm) {
    this->n = n;
    this->m = m;
    this->g_nm = g_nm;
    this->h_nm = h_nm;
    this->g_dot_nm = g_dot_nm;
    this->h_dot_nm = h_dot_nm;
}

int MagneticModelCoefficients::getN() const {
    return n;
}

int MagneticModelCoefficients::getM() const {
    return m;
}

double MagneticModelCoefficients::getG() const {
    return g_nm;
}

double MagneticModelCoefficients::getH() const {
    return h_nm;
}

double MagneticModelCoefficients::getGDot() const {
    return g_dot_nm;
}

double MagneticModelCoefficients::getHDot() const {
    return h_dot_nm;
}

MagneticModelCoefficients::MagneticModelCoefficients(const MagneticModelCoefficients &o) :
        n(o.getN()), m(o.getM()), g_nm(o.getG()), h_nm(o.getH()), g_dot_nm(o.getGDot()), h_dot_nm(o.getHDot()) {
}

std::unique_ptr<MagneticModelCoefficients>
MagneticModelCoefficients::getPointOnDate(double modelEpoch, const boost::gregorian::date &date) const {
    return std::unique_ptr<MagneticModelCoefficients>();
}
