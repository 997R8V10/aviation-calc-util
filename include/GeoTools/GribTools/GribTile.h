//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_GRIBTILE_H
#define AVIATION_CALC_UTIL_GRIBTILE_H

#ifdef _LIBRARY
#include <vector>
#include <mutex>
#include "FileSystem.h"
#endif
#include "GeoTools/GeoPoint.h"
#include "GeoTools/GeoTile.h"
#include <string>
#include <memory>
#include "GeoTools/GribTools/GribDataPoint.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "aviationcalc_exports.h"

using namespace std;
using namespace AviationCalcUtil::GeoTools;
using namespace boost::gregorian;
using namespace boost::posix_time;

namespace AviationCalcUtil::GeoTools::GribTools {
    class GribTile : public GeoTile {
#ifdef _LIBRARY
    private:
        static vector<shared_ptr<GribTile>> gribTileList;
        static mutex gribTileListLock;
        ptime forecastDateUtc;
        vector<shared_ptr<GribDataPoint>> dataPoints;
        mutable mutex gribDataListLock;
        bool downloaded;

        ptime getOffsetDateUtc() const;
        short getCycle() const;
        short getForecastHour() const;
        std::filesystem::path getGribPath() const;
        string getGribDateString() const;
        string getCycleString() const;
        string getForecastHourString() const;
        void extractData();
        void downloadTile();
        string getDownloadUrl() const;
#endif
    public:
        AVIATIONCALC_EXPORT static shared_ptr<const GribTile> findOrCreateGribTile(const GeoPoint &pos, const ptime &dateTime);

        AVIATIONCALC_EXPORT GribTile(const GeoPoint &pos, ptime dateTime);

        AVIATIONCALC_EXPORT ptime getForecastDateUtc() const;
        AVIATIONCALC_EXPORT string getGribFileName() const;
        AVIATIONCALC_EXPORT shared_ptr<const GribDataPoint> getClosestPoint(const GeoPoint &acftPos) const;
        AVIATIONCALC_EXPORT bool isValid(const ptime &dateTime) const;
        AVIATIONCALC_EXPORT bool equals(const GribTile &o) const;

        AVIATIONCALC_EXPORT ~GribTile();
    };
}

extern "C"{
    extern AVIATIONCALC_EXPORT shared_ptr<const GribTile> *GribTileFindOrCreateGribTile(GeoPoint *pos, uint64_t dateTime);
    extern AVIATIONCALC_EXPORT shared_ptr<const GribTile> *CreateGribTile(GeoPoint *pos, uint64_t dateTime);
    extern AVIATIONCALC_EXPORT void DisposeGribTile(shared_ptr<const GribTile> *tile);
    extern AVIATIONCALC_EXPORT uint64_t GribTileGetForecastDateUtc(shared_ptr<const GribTile> *tile);
    extern AVIATIONCALC_EXPORT const char *GribTileGetGribFileName(shared_ptr<const GribTile> *tile);
    extern AVIATIONCALC_EXPORT GribDataPoint *GribTileGetClosestPoint(shared_ptr<const GribTile> *tile, GeoPoint *acftPos);
    extern AVIATIONCALC_EXPORT bool GribTileIsValid(shared_ptr<const GribTile> *tile, uint64_t dateTime);
    extern AVIATIONCALC_EXPORT bool GribTileEquals(shared_ptr<const GribTile> *tile, shared_ptr<const GribTile> *o);
    extern AVIATIONCALC_EXPORT bool GribTileIsPointInTile(shared_ptr<const GribTile> *tile, GeoPoint *point);
    extern AVIATIONCALC_EXPORT GeoPoint *GribTileGetCenterPoint(shared_ptr<const GribTile> *tile);
    extern AVIATIONCALC_EXPORT double GribTileGetBottomLat(shared_ptr<const GribTile> *tile);
    extern AVIATIONCALC_EXPORT double GribTileGetTopLat(shared_ptr<const GribTile> *tile);
    extern AVIATIONCALC_EXPORT double GribTileGetLeftLon(shared_ptr<const GribTile> *tile);
    extern AVIATIONCALC_EXPORT double GribTileGetRightLon(shared_ptr<const GribTile> *tile);
}

#endif //AVIATION_CALC_UTIL_GRIBTILE_H
