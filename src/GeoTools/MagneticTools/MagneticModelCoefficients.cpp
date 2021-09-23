//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "GeoTools/MagneticTools/MagneticModelCoefficients.h"
#include "GeoTools/MagneticTools/MagneticUtil.h"
#include "InteropTools/InteropUtil.h"

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
    double dateModelEpoch = MagneticUtil::getEpochYear(date);
    double g_nm_t = g_nm + (dateModelEpoch - modelEpoch) * g_dot_nm;
    double h_nm_t = h_nm + (dateModelEpoch - modelEpoch) * h_dot_nm;

    return std::make_unique<MagneticModelCoefficients>(n, m, g_nm_t, h_nm_t, g_dot_nm, h_dot_nm);
}
/*
MagneticModelCoefficients *CopyMagModelCoeffs(MagneticModelCoefficients *o) {
    if (o == NULL){
        return NULL;
    }
    return new MagneticModelCoefficients(*o);
}

MagneticModelCoefficients *
CreateMagModelCoeffs(int n, int m, double g_nm, double h_nm, double g_dot_nm, double h_dot_nm) {
    return new MagneticModelCoefficients(n, m, g_nm, h_nm, g_dot_nm, h_dot_nm);
}

void DisposeMagModelCoeffs(MagneticModelCoefficients *coeffs) {
    if (coeffs != NULL){
        delete coeffs;
        coeffs = NULL;
    }
}

int MagModelCoeffsGetN(MagneticModelCoefficients *coeffs) {
    if (coeffs == NULL){
        return -1;
    }
    return coeffs->getN();
}

int MagModelCoeffsGetM(MagneticModelCoefficients *coeffs) {
    if (coeffs == NULL){
        return -1;
    }
    return coeffs->getM();
}

double MagModelCoeffsGetG(MagneticModelCoefficients *coeffs) {
    if (coeffs == NULL){
        return -1;
    }
    return coeffs->getG();
}

double MagModelCoeffsGetH(MagneticModelCoefficients *coeffs) {
    if (coeffs == NULL){
        return -1;
    }
    return coeffs->getH();
}

double MagModelCoeffsGetGDot(MagneticModelCoefficients *coeffs) {
    if (coeffs == NULL){
        return -1;
    }
    return coeffs->getGDot();
}

double MagModelCoeffsGetHDot(MagneticModelCoefficients *coeffs) {
    if (coeffs == NULL){
        return -1;
    }
    return coeffs->getHDot();
}

MagneticModelCoefficients *
MagModelCoeffsGetPointOnDate(MagneticModelCoefficients *coeffs, double modelEpoch, InteropDateStruct dateStruct) {
    if (coeffs == NULL){
        return NULL;
    }

    return coeffs->getPointOnDate(modelEpoch, InteropStructToBoostDate(dateStruct)).release();
}
*/