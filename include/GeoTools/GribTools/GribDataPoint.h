//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_GRIBDATAPOINT_H
#define AVIATION_CALC_UTIL_GRIBDATAPOINT_H

#include <GeoPoint.h>
#include <string>
#include "aviationcalc_exports.h"

namespace AviationCalcUtil::GeoTools::GribTools {
    class AVIATIONCALC_EXPORTS GribDataPoint {
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
        GribDataPoint(double lat, double lon, int level_hPa);

        double getDistanceFrom(const GeoPoint &pos) const;

        std::string toString() const;

        double getLatitude() const;

        double getLongitude() const;

        double getLongitudeNormalized() const;

        double getGeoPotentialHeightM() const;

        void setGeoPotentialHeight(double newGeoPotHtM);

        double getGeoPotentialHeightFt() const;

        int getLevelHPa() const;

        double getTempK() const;

        void setTempK(double newTempK);

        double getTempC() const;

        double getVMpers() const;

        void setVMpers(double newVMpers);

        double getUMpers() const;

        void setUMpers(double newUMpers);

        double getWindSpeedMpers() const;

        double getWindSpeedKts() const;

        double getWindDirRads() const;

        double getWindDirDegs() const;

        double getRelHumidity() const;

        void setRelHumidity(double newRelHumidity);

        double getSfcPressHPa() const;

        void setSfcPressHPa(double newSfcPressHPa);
    };
}

#endif //AVIATION_CALC_UTIL_GRIBDATAPOINT_H
