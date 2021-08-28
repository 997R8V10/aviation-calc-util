//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_GRIBDATAPOINT_H
#define AVIATION_CALC_UTIL_GRIBDATAPOINT_H

#include "GeoTools/GeoPoint.h"
#include <string>
#include "aviationcalc_exports.h"

namespace AviationCalcUtil::GeoTools::GribTools {
    class GribDataPoint {
    private:
        double latitude;
        double longitude;
        double geoPotHeight_M;
        int level_hPa;
        double temp_K;
        double v_mpers;
        double u_mpers;
        double rh;
        double sfcPress_hPa;

    public:
        AVIATIONCALC_EXPORT GribDataPoint(double lat, double lon, int level_hPa);

        AVIATIONCALC_EXPORT double getDistanceFrom(const GeoPoint &pos) const;

        AVIATIONCALC_EXPORT std::string toString() const;

        AVIATIONCALC_EXPORT double getLatitude() const;

        AVIATIONCALC_EXPORT double getLongitude() const;

        AVIATIONCALC_EXPORT double getLongitudeNormalized() const;

        AVIATIONCALC_EXPORT double getGeoPotentialHeightM() const;

        AVIATIONCALC_EXPORT void setGeoPotentialHeight(double newGeoPotHtM);

        AVIATIONCALC_EXPORT double getGeoPotentialHeightFt() const;

        AVIATIONCALC_EXPORT int getLevelHPa() const;

        AVIATIONCALC_EXPORT double getTempK() const;

        AVIATIONCALC_EXPORT void setTempK(double newTempK);

        AVIATIONCALC_EXPORT double getTempC() const;

        AVIATIONCALC_EXPORT double getVMpers() const;

        AVIATIONCALC_EXPORT void setVMpers(double newVMpers);

        AVIATIONCALC_EXPORT double getUMpers() const;

        AVIATIONCALC_EXPORT void setUMpers(double newUMpers);

        AVIATIONCALC_EXPORT double getWindSpeedMpers() const;

        AVIATIONCALC_EXPORT double getWindSpeedKts() const;

        AVIATIONCALC_EXPORT double getWindDirRads() const;

        AVIATIONCALC_EXPORT double getWindDirDegs() const;

        AVIATIONCALC_EXPORT double getRelHumidity() const;

        AVIATIONCALC_EXPORT void setRelHumidity(double newRelHumidity);

        AVIATIONCALC_EXPORT double getSfcPressHPa() const;

        AVIATIONCALC_EXPORT void setSfcPressHPa(double newSfcPressHPa);
    };
}

#endif //AVIATION_CALC_UTIL_GRIBDATAPOINT_H
