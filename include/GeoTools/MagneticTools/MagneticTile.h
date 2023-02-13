//
// Created by Prithvisagar Shivaraman on 9/2/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICTILE_H
#define AVIATION_CALC_UTIL_MAGNETICTILE_H

#include "GeoTools/GeoTile.h"
#include "GeoTools/MagneticTools/MagneticResult.h"
#include "aviationcalc_exports.h"
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>

#ifdef _LIBRARY
#include <vector>
#include <mutex>
#endif

using namespace AviationCalcUtil::GeoTools;
using namespace std;

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticTile : public GeoTile {
#ifdef _LIBRARY
    private:
        static vector<shared_ptr<MagneticTile>> magTileList;
        static mutex magTileListLock;

        boost::gregorian::date date;
        shared_ptr<MagneticResult> data;
#endif

    public:
        AVIATIONCALC_EXPORT static constexpr double RESOLUTION = 0.1;

        AVIATIONCALC_EXPORT static shared_ptr<const MagneticTile> findOrCreateTile(const GeoPoint &pos, const boost::gregorian::date &date);

        AVIATIONCALC_EXPORT MagneticTile(const GeoPoint &point, const boost::gregorian::date &date);

        AVIATIONCALC_EXPORT bool isValid(const boost::gregorian::date &date) const;

        AVIATIONCALC_EXPORT shared_ptr<const MagneticResult> getData() const;
    };
}

extern "C"{
    extern AVIATIONCALC_EXPORT double MagnetiTileGetResolution();
    extern AVIATIONCALC_EXPORT shared_ptr<const MagneticTile> *MagneticTileFindOrCreateTile(GeoPoint *pos, InteropDateStruct dStruct);
    extern AVIATIONCALC_EXPORT shared_ptr<const MagneticTile> *CreateMagneticTile(GeoPoint *point, InteropDateStruct dStruct);
    extern AVIATIONCALC_EXPORT void DisposeMagneticTile(shared_ptr<const MagneticTile> *tile);
    extern AVIATIONCALC_EXPORT bool MagneticTileIsValid(shared_ptr<const MagneticTile> *tile, InteropDateStruct dStruct);
    extern AVIATIONCALC_EXPORT shared_ptr<const MagneticResult> *MagneticTileGetData(shared_ptr<const MagneticTile> *tile);
    extern AVIATIONCALC_EXPORT bool MagneticTileIsPointInTile(shared_ptr<const MagneticTile> *tile, GeoPoint *point);
    extern AVIATIONCALC_EXPORT GeoPoint *MagneticTileGetCenterPoint(shared_ptr<const MagneticTile> *tile);
    extern AVIATIONCALC_EXPORT double MagneticTileGetBottomLat(shared_ptr<const MagneticTile> *tile);
    extern AVIATIONCALC_EXPORT double MagneticTileGetTopLat(shared_ptr<const MagneticTile> *tile);
    extern AVIATIONCALC_EXPORT double MagneticTileGetLeftLon(shared_ptr<const MagneticTile> *tile);
    extern AVIATIONCALC_EXPORT double MagneticTileGetRightLon(shared_ptr<const MagneticTile> *tile);
}

#endif //AVIATION_CALC_UTIL_MAGNETICTILE_H
