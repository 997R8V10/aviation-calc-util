//
// Created by pshivaraman on 2021-08-23.
//

#include "GribDataPoint.h"
#include <MathUtil.h>
#include <GeoUtil.h>
#include <sstream>

#define _USE_MATH_DEFINES

#include <cmath>

using namespace AviationCalcUtil::MathTools;
using namespace AviationCalcUtil::GeoTools;

GribTools::GribDataPoint::GribDataPoint(double lat, double lon, int level_hPa) {
    this->latitude = lat;
    this->longitude = lon;
    this->level_hPa = level_hPa;
}

double GribTools::GribDataPoint::getDistanceFrom(const GeoPoint &pos) const {
    double absHeightM = MathUtil::convertFeetToMeters(pos.getAlt());
    double acftGeoPotHeightM = GeoUtil::EARTH_RADIUS_M * absHeightM / (GeoUtil::EARTH_RADIUS_M + absHeightM);
    double flatDistNMi = GeoPoint::flatDistanceNMi(pos, GeoPoint(latitude, getLongitudeNormalized()));
    double altDistNMi = MathUtil::convertMetersToNauticalMiles(fabs(acftGeoPotHeightM - geoPotHeight_M));
    return sqrt(pow(flatDistNMi, 2) + pow(altDistNMi, 2));
}

string GribTools::GribDataPoint::toString() const {
    stringstream ss;

    ss << "Lat: " << latitude
       << " Lon: " << getLongitudeNormalized()
       << " Level: " << level_hPa << "hPa"
       << " Height: " << getGeoPotentialHeightFt() << "ft"
       << " Temp: " << getTempC() << "C"
       << " Wind: " << getWindDirDegs() << "@" << getWindSpeedKts() << "KT"
       << " RH: " << rh;
    return ss.str();
}

double GribTools::GribDataPoint::getLatitude() const {
    return latitude;
}

double GribTools::GribDataPoint::getLongitude() const {
    return longitude;
}

double GribTools::GribDataPoint::getLongitudeNormalized() const {
    return longitude > 180 ? longitude - 360 : longitude;
}

double GribTools::GribDataPoint::getGeoPotentialHeightM() const {
    return geoPotHeight_M;
}

void GribTools::GribDataPoint::setGeoPotentialHeight(double newGeoPotHtM) {
    geoPotHeight_M = newGeoPotHtM;
}

double GribTools::GribDataPoint::getGeoPotentialHeightFt() const {
    return MathUtil::convertMetersToFeet(geoPotHeight_M);
}

int GribTools::GribDataPoint::getLevelHPa() const {
    return level_hPa;
}

double GribTools::GribDataPoint::getTempK() const {
    return temp_K;
}

void GribTools::GribDataPoint::setTempK(double newTempK) {
    temp_K = newTempK;
}

double GribTools::GribDataPoint::getTempC() const {
    return temp_K == 0 ? -56.5 : temp_K - MathUtil::CONV_FACTOR_KELVIN_C;
}

double GribTools::GribDataPoint::getVMpers() const {
    return v_mpers;
}

void GribTools::GribDataPoint::setVMpers(double newVMpers) {
    v_mpers = newVMpers;
}

double GribTools::GribDataPoint::getUMpers() const {
    return u_mpers;
}

void GribTools::GribDataPoint::setUMpers(double newUMpers) {
    u_mpers = newUMpers;
}

double GribTools::GribDataPoint::getWindSpeedMpers() const {
    return sqrt(pow(u_mpers, 2) + pow(v_mpers, 2));
}

double GribTools::GribDataPoint::getWindSpeedKts() const {
    return getWindSpeedMpers() * 1.943844;
}

double GribTools::GribDataPoint::getWindDirRads() const {
    return atan2(-u_mpers, -v_mpers);
}

double GribTools::GribDataPoint::getWindDirDegs() const {
    return MathUtil::convertRadiansToDegrees(getWindDirRads());
}

double GribTools::GribDataPoint::getRelHumidity() const {
    return rh;
}

void GribTools::GribDataPoint::setRelHumidity(double newRelHumidity) {
    rh = newRelHumidity;
}

double GribTools::GribDataPoint::getSfcPressHPa() const {
    return sfcPress_hPa;
}

void GribTools::GribDataPoint::setSfcPressHPa(double newSfcPressHPa) {
    sfcPress_hPa = newSfcPressHPa;
}
