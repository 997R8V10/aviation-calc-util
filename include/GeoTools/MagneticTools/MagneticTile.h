//
// Created by Prithvisagar Shivaraman on 9/2/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICTILE_H
#define AVIATION_CALC_UTIL_MAGNETICTILE_H

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticTile {
#ifdef _LIBRARY
        static vector<shared_ptr<MagneticTile>> gribTileList;
        static mutex gribTileListLock;
        short topLat;
        short bottomLat;
        short leftLon;
        short rightLon;
#endif
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICTILE_H
