//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_GRIBDATAPOINT_H
#define AVIATION_CALC_UTIL_GRIBDATAPOINT_H

#include "GeoTools/GeoPoint.h"
#include "GeoTools/AtmosUtil.h"
#include <string>
#include "aviationcalc_exports.h"

using namespace AviationCalcUtil::GeoTools;

namespace AviationCalcUtil::GeoTools::GribTools {
    class GribDataPoint {
    private:
        double latitude{};
        double longitude{};
        double geoPotHeight_M{};
        int level_hPa{};
        double temp_K{};
        double v_mpers{};
        double u_mpers{};
        double rh{};
        double sfcPress_hPa{AtmosUtil::ISA_STD_PRES_hPa};

    public:
        AVIATIONCALC_EXPORT GribDataPoint(double lat, double lon, int level_hPa);

        AVIATIONCALC_EXPORT GribDataPoint(const GribDataPoint& point);

        AVIATIONCALC_EXPORT double getDistanceFrom(const GeoPoint &pos) const;

        AVIATIONCALC_EXPORT std::string toString() const;

        AVIATIONCALC_EXPORT double getLatitude() const;

        AVIATIONCALC_EXPORT double getLongitude() const;

        AVIATIONCALC_EXPORT double getLongitudeNormalized() const;

        AVIATIONCALC_EXPORT double getGeoPotentialHeightM() const;

        AVIATIONCALC_EXPORT void setGeoPotentialHeightM(double newGeoPotHtM);

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

using namespace GribTools;

extern "C"{
    extern AVIATIONCALC_EXPORT GribDataPoint *CreateGribDataPoint(double lat, double lon, int level_hPa);
    extern AVIATIONCALC_EXPORT void DisposeGribDataPoint(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetDistanceFrom(GribDataPoint *point, GeoPoint *pos);
    extern AVIATIONCALC_EXPORT const char *GribDataPointToString(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetLatitude(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetLongitude(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetLongitudeNormalized(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetGeoPotentialHeightM(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT void GribDataPointSetGeoPotentialHeightM(GribDataPoint *point, double newGeoPotHtM);
    extern AVIATIONCALC_EXPORT double GribDataPointGetGeoPotentialHeightFt(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT int GribDataPointGetLevelHPa(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetTempK(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT void GribDataPointSetTempK(GribDataPoint *point, double newTempK);
    extern AVIATIONCALC_EXPORT double GribDataPointGetTempC(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetVMpers(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT void GribDataPointSetVMpers(GribDataPoint *point, double newVMpers);
    extern AVIATIONCALC_EXPORT double GribDataPointGetUMpers(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT void GribDataPointSetUMpers(GribDataPoint *point, double newUMpers);
    extern AVIATIONCALC_EXPORT double GribDataPointGetWindSpeedMpers(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetWindSpeedKts(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetWindDirRads(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetWindDirDegs(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT double GribDataPointGetRelHumidity(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT void GribDataPointSetRelHumidity(GribDataPoint *point, double newRelHumidity);
    extern AVIATIONCALC_EXPORT double GribDataPointGetSfcPressHPa(GribDataPoint *point);
    extern AVIATIONCALC_EXPORT void GribDataPointSetSfcPressHPa(GribDataPoint *point, double newSfcPressHPa);
}
#endif //AVIATION_CALC_UTIL_GRIBDATAPOINT_H
