//
// Created by pshivaraman on 2021-08-23.
//

#include "GeoTools/GribTools/GribDataPoint.h"
#include "MathTools/MathUtil.h"
#include "GeoTools/GeoUtil.h"
#include <sstream>
#include "InteropTools/InteropUtil.h"

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

void GribTools::GribDataPoint::setGeoPotentialHeightM(double newGeoPotHtM) {
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
    return GeoUtil::normalizeHeading(MathUtil::convertRadiansToDegrees(getWindDirRads()));
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

GribTools::GribDataPoint::GribDataPoint(const GribTools::GribDataPoint &point): GribDataPoint(point.getLatitude(), point.getLongitude(), point.getLevelHPa()) {
    geoPotHeight_M = point.getGeoPotentialHeightM();
    temp_K = point.getTempK();
    v_mpers = point.getVMpers();
    u_mpers = point.getUMpers();
    rh = point.getRelHumidity();
    sfcPress_hPa = point.getSfcPressHPa();
}

GribDataPoint *CreateGribDataPoint(double lat, double lon, int level_hPa) {
    return new GribDataPoint(lat, lon, level_hPa);
}

void DisposeGribDataPoint(GribDataPoint *point) {
    if (point != NULL){
        delete point;
        point = NULL;
    }
}

double GribDataPointGetDistanceFrom(GribDataPoint *point, GeoPoint *pos) {
    if (point == NULL || pos == NULL){
        return -1;
    }
    return point->getDistanceFrom(*pos);
}

const char *GribDataPointToString(GribDataPoint *point) {
    if (point == NULL){
        return NULL;
    }
    return InteropCppStrToCStr(point->toString());
}

double GribDataPointGetLatitude(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getLatitude();
}

double GribDataPointGetLongitude(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getLongitude();
}

double GribDataPointGetLongitudeNormalized(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getLongitudeNormalized();
}

double GribDataPointGetGeoPotentialHeightM(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getGeoPotentialHeightM();
}

void GribDataPointSetGeoPotentialHeightM(GribDataPoint *point, double newGeoPotHtM) {
    if (point != NULL){
        point->setGeoPotentialHeightM(newGeoPotHtM);
    }
}

double GribDataPointGetGeoPotentialHeightFt(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getGeoPotentialHeightFt();
}

int GribDataPointGetLevelHPa(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getLevelHPa();
}

double GribDataPointGetTempK(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getTempK();
}

void GribDataPointSetTempK(GribDataPoint *point, double newTempK) {
    if (point != NULL){
        point->setTempK(newTempK);
    }
}

double GribDataPointGetTempC(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getTempC();
}

double GribDataPointGetVMpers(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getVMpers();
}

void GribDataPointSetVMpers(GribDataPoint *point, double newVMpers) {
    if (point != NULL){
        point->setVMpers(newVMpers);
    }
}

double GribDataPointGetUMpers(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getUMpers();
}

void GribDataPointSetUMpers(GribDataPoint *point, double newUMpers) {
    if (point != NULL){
        point->setUMpers(newUMpers);
    }
}

double GribDataPointGetWindSpeedMpers(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getWindSpeedMpers();
}

double GribDataPointGetWindSpeedKts(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getWindSpeedKts();
}

double GribDataPointGetWindDirRads(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getWindDirRads();
}

double GribDataPointGetWindDirDegs(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getWindDirDegs();
}

double GribDataPointGetRelHumidity(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getRelHumidity();
}

void GribDataPointSetRelHumidity(GribDataPoint *point, double newRelHumidity) {
    if (point != NULL){
        point->setRelHumidity(newRelHumidity);
    }
}

double GribDataPointGetSfcPressHPa(GribDataPoint *point) {
    if (point == NULL){
        return -1;
    }

    return point->getSfcPressHPa();
}

void GribDataPointSetSfcPressHPa(GribDataPoint *point, double newSfcPressHPa) {
    if (point != NULL){
        point->setSfcPressHPa(newSfcPressHPa);
    }
}
