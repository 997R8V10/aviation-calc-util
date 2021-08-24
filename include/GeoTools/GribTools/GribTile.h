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
#include <GribDataPoint.h>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace AviationCalcUtil::GeoTools;
using namespace boost::gregorian;
using namespace boost::posix_time;

namespace AviationCalcUtil::GeoTools::GribTools {
    class GribTile {
#ifdef _LIBRARY
    private:
        static vector<GribTile *> *gribTileList;
        static mutex gribTileListLock;
        short topLat;
        short bottomLat;
        short leftLon;
        short rightLon;
        ptime forecastDateUtc;
        vector<GribDataPoint *> *dataPoints;
        mutex gribDataListLock;
        bool downloaded;

        ptime getOffsetDateUtc();
        short getCycle();
        short getForecastHour();
        string getGribDateString();
        string getCycleString();
        string getForecastHourString();
        void extractData();
        void downloadTile();
        string getDownloadUrl();
#endif
    public:
        static GribTile *findOrCreateGribTile(GeoPoint *pos, ptime dateTime);

        GribTile(double lat, double lon, ptime dateTime);

        short getTopLat();
        short getBottomLat();
        short getLeftLon();
        short getRightLon();
        ptime getForecastDateUtc();
        string getGribFileName();
        GribDataPoint *getClosestPoint(GeoPoint *acftPos);
        bool isValid(ptime dateTime);
        bool isAcftInside(GeoPoint *pos);
        bool equals(GribTile *o);

        ~GribTile();
    };
}

#endif //AVIATION_CALC_UTIL_GRIBTILE_H
