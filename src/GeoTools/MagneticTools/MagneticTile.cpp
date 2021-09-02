//
// Created by pshivaraman on 2021-09-02.
//

#define _LIBRARY
#include "GeoTools/MagneticTools/MagneticTile.h"
#include "GeoTools/MagneticTools/MagneticUtil.h"

using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace std;

std::vector<std::shared_ptr<MagneticTile>> MagneticTile::magTileList{};
mutex MagneticTile::magTileListLock;

shared_ptr<const MagneticResult> MagneticTile::getData() const {
    return data;
}

shared_ptr<const MagneticTile> MagneticTile::findOrCreateTile(const GeoPoint &pos, const boost::gregorian::date &date) {
    if (std::isnan(pos.getLat()) || std::isnan(pos.getLon())) {
        return nullptr;
    }

    const lock_guard<mutex> gt_lock(magTileListLock);

    // Look for tile
    for (const shared_ptr<MagneticTile> &tile : magTileList) {
        if ((*tile) == pos && tile->isValid(date)) {
            return tile;
        }
    }

    // Create if not found
    auto newTile = std::make_shared<MagneticTile>(pos, date);
    magTileList.push_back(newTile);

    return newTile;
}

MagneticTile::MagneticTile(const GeoPoint &point, const boost::gregorian::date &date) : GeoTile(point, RESOLUTION) {
    this->date = date;
    data = MagneticUtil::getMagneticField(*getCenterPoint(), date);
}

bool MagneticTile::isValid(const boost::gregorian::date &date) const{
    return this->date == date;
}
