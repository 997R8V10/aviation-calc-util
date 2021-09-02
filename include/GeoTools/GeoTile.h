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

        shared_ptr<const GeoPoint> getCenterPoint() const;

        double getBottomLat() const;

        double getTopLat() const;

        double getLeftLon() const;

        double getRightLon() const;
    };
}

#endif //AVIATION_CALC_UTIL_GEOTILE_H
