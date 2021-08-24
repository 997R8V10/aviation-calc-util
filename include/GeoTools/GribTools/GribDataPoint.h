//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_GRIBDATAPOINT_H
#define AVIATION_CALC_UTIL_GRIBDATAPOINT_H

#include <GeoPoint.h>
#include <string>

using namespace std;

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
        GribDataPoint(double lat, double lon, int level_hPa);

        double getDistanceFrom(GeoPoint *pos);
        string toString();

        double getLatitude();
        double getLongitude();
        double getLongitudeNormalized();
        double getGeoPotentialHeightM();
        void setGeoPotentialHeight(double newGeoPotHtM);
        double getGeoPotentialHeightFt();
        int getLevelHPa();
        double getTempK();
        void setTempK(double newTempK);
        double getTempC();
        double getVMpers();
        void setVMpers(double newVMpers);
        double getUMpers();
        void setUMpers(double newUMpers);
        double getWindSpeedMpers();
        double getWindSpeedKts();
        double getWindDirRads();
        double getWindDirDegs();
        double getRelHumidity();
        void setRelHumidity(double newRelHumidity);
        double getSfcPressHPa();
        void setSfcPressHPa(double newSfcPressHPa);
    };
}

#endif //AVIATION_CALC_UTIL_GRIBDATAPOINT_H
