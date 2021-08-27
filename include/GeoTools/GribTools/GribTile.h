//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_GRIBTILE_H
#define AVIATION_CALC_UTIL_GRIBTILE_H

#ifdef _LIBRARY
#include <vector>
#include <mutex>
#endif
#include <GeoPoint.h>
#include <string>
#include <memory>
#include <GribDataPoint.h>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "aviationcalc_exports.h"

using namespace std;
using namespace AviationCalcUtil::GeoTools;
using namespace boost::gregorian;
using namespace boost::posix_time;

namespace AviationCalcUtil::GeoTools::GribTools {
    class GribTile {
#ifdef _LIBRARY
    private:
        static vector<shared_ptr<GribTile>> gribTileList;
        static mutex gribTileListLock;
        short topLat;
        short bottomLat;
        short leftLon;
        short rightLon;
        ptime forecastDateUtc;
        vector<shared_ptr<GribDataPoint>> dataPoints;
        mutex gribDataListLock;
        bool downloaded;

        ptime getOffsetDateUtc() const;
        short getCycle() const;
        short getForecastHour() const;
        string getGribDateString() const;
        string getCycleString() const;
        string getForecastHourString() const;
        void extractData();
        void downloadTile();
        string getDownloadUrl() const;
#endif
    public:
        AVIATIONCALC_EXPORT static shared_ptr<const GribTile> findOrCreateGribTile(const GeoPoint &pos, const ptime &dateTime);

        AVIATIONCALC_EXPORT GribTile(double lat, double lon, ptime dateTime);

        AVIATIONCALC_EXPORT short getTopLat() const;
        AVIATIONCALC_EXPORT short getBottomLat() const;
        AVIATIONCALC_EXPORT short getLeftLon() const;
        AVIATIONCALC_EXPORT short getRightLon() const;
        AVIATIONCALC_EXPORT ptime getForecastDateUtc() const;
        AVIATIONCALC_EXPORT string getGribFileName() const;
        AVIATIONCALC_EXPORT shared_ptr<const GribDataPoint> getClosestPoint(const GeoPoint &acftPos);
        AVIATIONCALC_EXPORT bool isValid(const ptime &dateTime) const;
        AVIATIONCALC_EXPORT bool isAcftInside(const GeoPoint &pos) const;
        AVIATIONCALC_EXPORT bool equals(const GribTile &o) const;

        AVIATIONCALC_EXPORT ~GribTile();
    };
}

#endif //AVIATION_CALC_UTIL_GRIBTILE_H
