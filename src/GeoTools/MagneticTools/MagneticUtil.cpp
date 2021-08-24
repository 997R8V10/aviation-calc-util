//
// Created by Prithvisagar Shivaraman on 8/24/21.
//
#define _LIBRARY
#include "MagneticUtil.h"

using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace AviationCalcUtil::GeoTools;

const double MagneticUtil::EARTH_WGS84_SEMI_MAJOR_AXIS = 6378137.0;
const double MagneticUtil::EARTH_WGS84_RECIPROCAL_FLATTENING = 298.257223563;
const int MagneticUtil::WMM_EXPANSION = 12;
const double MagneticUtil::GEOMAGNETIC_REFERENCE_RADIUS = 6371200.0;
std::mutex MagneticUtil::modelLock;

void MagneticUtil::loadData() {

}

MagneticResult *MagneticUtil::getMagneticField(GeoPoint *point, boost::gregorian::date date) {
    return nullptr;
}

double MagneticUtil::getEpochYear(boost::gregorian::date date) {
    return 0;
}

void MagneticUtil::getSpherical(GeoPoint *point, double &lambda, double &phiPrime, double &r) {

}
