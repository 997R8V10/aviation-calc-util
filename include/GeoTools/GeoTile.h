//
// Created by pshivaraman on 2021-09-02.
//

#ifndef AVIATION_CALC_UTIL_GEOTILE_H
#define AVIATION_CALC_UTIL_GEOTILE_H

#include "GeoPoint.h"
#include "aviationcalc_exports.h"
#include <memory>

using namespace std;

namespace AviationCalcUtil::GeoTools {
    class AVIATIONCALC_EXPORT GeoTile {
    private:
        double botLat;
        double topLat;
        double leftLon;
        double rightLon;

    public:
        GeoTile(const GeoPoint &point, double resolutionDegs);

        GeoTile(double botLat, double topLat, double leftLon, double rightLon);

        GeoTile(const GeoTile &tile);

        bool isPointInTile(const GeoPoint &point) const;

        bool operator==(const GeoPoint &point) const;

        unique_ptr<GeoPoint> getCenterPoint() const;

        double getBottomLat() const;

        double getTopLat() const;

        double getLeftLon() const;

        double getRightLon() const;
    };
}
/*
using namespace AviationCalcUtil::GeoTools;

extern "C" {
extern AVIATIONCALC_EXPORT GeoTile *CreateGeoTile(double botLat, double topLat, double leftLon, double rightLon);
extern AVIATIONCALC_EXPORT GeoTile *CreateGeoTileFromGeoPoint(GeoPoint *point, double resolutionDegs);
extern AVIATIONCALC_EXPORT GeoTile *CopyGeoTile(GeoTile *tile);
extern AVIATIONCALC_EXPORT void DisposeGeoTile(GeoTile *tile);
extern AVIATIONCALC_EXPORT bool GeoTileIsPointInTile(GeoTile *tile, GeoPoint *point);
extern AVIATIONCALC_EXPORT GeoPoint *GeoTileGetCenterPoint(GeoTile *tile);
extern AVIATIONCALC_EXPORT double GeoTileGetBottomLat(GeoTile *tile);
extern AVIATIONCALC_EXPORT double GeoTileGetTopLat(GeoTile *tile);
extern AVIATIONCALC_EXPORT double GeoTileGetLeftLon(GeoTile *tile);
extern AVIATIONCALC_EXPORT double GeoTileGetRightLon(GeoTile *tile);
}
*/
#endif //AVIATION_CALC_UTIL_GEOTILE_H
