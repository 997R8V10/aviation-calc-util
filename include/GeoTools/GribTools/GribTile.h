//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_GRIBTILE_H
#define AVIATION_CALC_UTIL_GRIBTILE_H
#ifdef _WIN32
#include <windows.h>
#include <urlmon.h>
#endif

#ifdef __unix__
#include <curl/curl.h>
#endif

#include <vector>
#include <mutex>
#include <GeoPoint.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <thread>
#include "eccodes.h"
#include <GribDataPoint.h>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace AviationCalcUtil::GeoTools;
using namespace boost::gregorian;
using namespace boost::posix_time;

namespace AviationCalcUtil::GeoTools::GribTools {
    class GribTile {
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
