//
// Created by pshivaraman on 2021-09-02.
//

#include "GeoTools/GeoTile.h"
#include "GeoTools/GeoUtil.h"

#define _USE_MATH_DEFINES

#include <cmath>

using namespace AviationCalcUtil::GeoTools;
using namespace std;

GeoTile::GeoTile(const GeoPoint &point, double resolutionDegs) {
    // Get indexes for the specified resolution and floor it
    int maxLatIndex = static_cast<int>(floor(90 / resolutionDegs));
    int maxLonIndex = static_cast<int>(floor(180 / resolutionDegs));
    int latIndex = static_cast<int>(floor(point.getLat() / resolutionDegs));
    int lonIndex = static_cast<int>(floor(point.getLon() / resolutionDegs));

    // Check index
    if (latIndex == maxLatIndex) {
        latIndex--;
    }

    if (lonIndex == maxLonIndex) {
        lonIndex--;
    }

    // Set bounds
    botLat = latIndex * resolutionDegs;
    topLat = (latIndex + 1) * resolutionDegs;
    leftLon = lonIndex * resolutionDegs;
    rightLon = (lonIndex + 1) * resolutionDegs;
}

GeoTile::GeoTile(double botLat, double topLat, double leftLon, double rightLon) {
    if (rightLon < leftLon) {
        rightLon = leftLon;
    }

    if (topLat < botLat) {
        topLat = botLat;
    }

    this->botLat = fmin(fmax(botLat, -90), 90);
    this->topLat = fmin(fmax(topLat, -90), 90);
    this->leftLon = GeoUtil::normalizeLongitude(leftLon);
    this->rightLon = GeoUtil::normalizeLongitude(rightLon);
}

GeoTile::GeoTile(const GeoTile &tile) : GeoTile(tile.getBottomLat(), tile.getTopLat(), tile.getLeftLon(),
                                                tile.getRightLon()) {
}

bool GeoTile::isPointInTile(const GeoPoint &point) const {
    return point.getLat() >= botLat && point.getLat() <= topLat &&
           point.getLon() >= leftLon && point.getLon() <= rightLon;
}

bool GeoTile::operator==(const GeoPoint &point) const {
    return isPointInTile(point);
}

double GeoTile::getBottomLat() const {
    return botLat;
}

double GeoTile::getTopLat() const {
    return topLat;
}

double GeoTile::getLeftLon() const {
    return leftLon;
}

double GeoTile::getRightLon() const {
    return rightLon;
}

unique_ptr<GeoPoint> GeoTile::getCenterPoint() const {
    double centerLat = (botLat + topLat) / 2;
    double centerLon = (leftLon + rightLon) / 2;
    return make_unique<GeoPoint>(centerLat, centerLon);
}
