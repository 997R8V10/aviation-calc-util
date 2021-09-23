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
/*
double MagnetiTileGetResolution() {
    return MagneticTile::RESOLUTION;
}

shared_ptr<const MagneticTile> *MagneticTileFindOrCreateTile(GeoPoint *pos, InteropDateStruct dStruct) {
    if (pos == NULL){
        return NULL;
    }
    auto retPtr = MagneticTile::findOrCreateTile(*pos, InteropStructToBoostDate(dStruct));

    if (retPtr == NULL){
        return NULL;
    }

    return new std::shared_ptr<const MagneticTile>(retPtr);
}

shared_ptr<const MagneticTile> *CreateMagneticTile(GeoPoint *point, InteropDateStruct dStruct) {
    if (point == NULL){
        return NULL;
    }
    auto retPtr = std::make_shared<const MagneticTile>(*point, InteropStructToBoostDate(dStruct));

    if (retPtr == NULL){
        return NULL;
    }

    return new std::shared_ptr<const MagneticTile>(retPtr);
}

void DisposeMagneticTile(shared_ptr<const MagneticTile> *tile) {
    if (tile != NULL){
        delete tile;
        tile = NULL;
    }
}

bool MagneticTileIsValid(shared_ptr<const MagneticTile> *tile, InteropDateStruct dStruct) {
    if (tile == NULL){
        return false;
    }

    return tile->get()->isValid(InteropStructToBoostDate(dStruct));
}

shared_ptr<const MagneticResult> *MagneticTileGetData(shared_ptr<const MagneticTile> *tile) {
    if (tile == NULL){
        return NULL;
    }

    auto resPtr = tile->get()->getData();
    if (resPtr == NULL){
        return NULL;
    }

    return new std::shared_ptr<const MagneticResult>(resPtr);
}

bool MagneticTileIsPointInTile(shared_ptr<const MagneticTile> *tile, GeoPoint *point) {
    if (tile == nullptr || point == nullptr){
        return false;
    }

    return tile->get()->isPointInTile(*point);
}

GeoPoint *MagneticTileGetCenterPoint(shared_ptr<const MagneticTile> *tile) {
    if (tile == nullptr){
        return nullptr;
    }

    return tile->get()->getCenterPoint().release();
}

double MagneticTileGetBottomLat(shared_ptr<const MagneticTile> *tile) {
    if (tile == nullptr){
        return -1;
    }
    return (*tile)->getBottomLat();
}

double MagneticTileGetTopLat(shared_ptr<const MagneticTile> *tile) {
    if (tile == nullptr){
        return -1;
    }
    return (*tile)->getTopLat();
}

double MagneticTileGetLeftLon(shared_ptr<const MagneticTile> *tile) {
    if (tile == nullptr){
        return -1;
    }
    return (*tile)->getLeftLon();
}

double MagneticTileGetRightLon(shared_ptr<const MagneticTile> *tile) {
    if (tile == nullptr){
        return -1;
    }
    return (*tile)->getRightLon();
}
*/