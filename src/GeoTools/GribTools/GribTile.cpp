//
// Created by pshivaraman on 2021-08-23.
//
#define _LIBRARY

#ifdef _WIN32

#include <windows.h>
#include <urlmon.h>

#endif

#ifdef __unix__
#include <curl/curl.h>
#endif

#include "GribTile.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <thread>
#include "eccodes.h"

#define MAX_VAL_LEN 1024

using namespace AviationCalcUtil::GeoTools::GribTools;

std::vector<std::shared_ptr<GribTile>> GribTile::gribTileList{};
mutex GribTile::gribTileListLock;

std::shared_ptr<const GribTile> GribTile::findOrCreateGribTile(const GeoPoint &pos, const ptime &dateTime) {
    gribTileListLock.lock();

    // Look for tile
    for (const shared_ptr<GribTile> &tile : gribTileList) {
        if (tile->isAcftInside(pos) && tile->isValid(dateTime)) {
            gribTileListLock.unlock();
            return tile;
        }
    }

    // Create if not found
    auto newTile = std::make_shared<GribTile>(pos.getLat(), pos.getLon(), dateTime);
    gribTileList.push_back(newTile);
    gribTileListLock.unlock();

    return newTile;
}

ptime GribTile::getOffsetDateUtc() const {
    return forecastDateUtc - hours(6);
}

short GribTile::getCycle() const {
    return (short) (getOffsetDateUtc().time_of_day().hours() * 6 / 6);
}

short GribTile::getForecastHour() const {
    return (short) (getOffsetDateUtc().time_of_day().hours() - getCycle() + 6);
}

string GribTile::getGribDateString() const {
    static std::locale loc(std::cout.getloc(),
                           new wtime_facet(L"%Y%m%d"));
    stringstream ss;
    ss.imbue(loc);
    ss << getOffsetDateUtc();
    return ss.str();
}

string GribTile::getCycleString() const {
    stringstream ss;
    ss << setw(2) << setfill('0') << getCycle();
    return ss.str();
}

string GribTile::getForecastHourString() const {
    stringstream ss;
    ss << setw(3) << setfill('0') << getForecastHour();
    return ss.str();
}

void GribTile::extractData() {
    std::vector<std::shared_ptr<GribDataPoint>> sfcValues {};

    // Extract Data from Grib File
    int err = 0;
    double missingValue = 1e+20; /* A value out of range */
    long bitmapPresent = 0;
    string filename = getGribFileName();

    /* Message handle. Required in all the ecCodes calls acting on a message.*/
    codes_handle *h = NULL;

    FILE *file = fopen(filename.c_str(), "rb");
    if (!file) {
        fprintf(stderr, "Error: unable to open file %s\n", filename.c_str());
        return;
    }

    /* Loop on all the messages in a file.*/
    while ((h = codes_handle_new_from_file(0, file, PRODUCT_GRIB, &err)) != NULL) {
        /* Check of errors after reading a message. */
        if (err != CODES_SUCCESS) CODES_CHECK(err, 0);

        /* Check if a bitmap applies */
        CODES_CHECK(codes_get_long(h, "bitmapPresent", &bitmapPresent), 0);

        if (bitmapPresent) {
            /* Set the double representing the missing value in the field. */
            /* Choose a missingValue that does not correspond to any real value in the data array */
            CODES_CHECK(codes_set_double(h, "missingValue", missingValue), 0);
        }

        // Get Level Type
        size_t len = 0;
        CODES_CHECK(codes_get_length(h, "levType", &len), 0);
        char *levelTypeCArr = new char[len];
        codes_get_string(h, "levType", levelTypeCArr, &len);
        string levelType(levelTypeCArr);
        delete[] levelTypeCArr;

        // Get Short Name
        CODES_CHECK(codes_get_length(h, "shortName", &len), 0);
        char *shortNameCArr = new char[len];
        codes_get_string(h, "shortName", shortNameCArr, &len);
        string shortName(shortNameCArr);
        delete[] shortNameCArr;

        // Get level
        long level = -1;

        bool isSurfaceMsg = false;
        bool isIsobaricMsg = false;
        if (levelType == "isobaricInhPa") {
            isIsobaricMsg = true;

            CODES_CHECK(codes_get_long(h, "level", &level), 0);
        } else if (levelType == "meanSea" && shortName == "prmsl") {
            isSurfaceMsg = true;
        }

        if (isIsobaricMsg || isSurfaceMsg) {
            // Loop through values
            codes_iterator *iter = codes_grib_iterator_new(h, 0, &err);
            if (err != CODES_SUCCESS) CODES_CHECK(err, 0);

            double lat, lon, value;
            while (codes_grib_iterator_next(iter, &lat, &lon, &value)) {
                if (bitmapPresent && value == missingValue) {
                    continue;
                }

                // Get GRID Point if it exists
                std::shared_ptr<GribDataPoint> foundPoint(nullptr);
                if (isIsobaricMsg) {
                    gribDataListLock.lock();
                    for (const std::shared_ptr<GribDataPoint> &pt : dataPoints) {
                        if (pt->getLatitude() == lat && pt->getLongitude() == lon && pt->getLevelHPa() == level) {
                            foundPoint = pt;
                            break;
                        }
                    }
                    gribDataListLock.unlock();
                } else {
                    for (const std::shared_ptr<GribDataPoint> &pt : sfcValues) {
                        if (pt->getLatitude() == lat && pt->getLongitude() == lon && level == 0) {
                            foundPoint = pt;
                            break;
                        }
                    }
                }

                // Create point if it doesn't exist
                if (foundPoint == nullptr) {
                    foundPoint = std::make_shared<GribDataPoint>(lat, lon, level);

                    if (isIsobaricMsg) {
                        gribDataListLock.lock();
                        dataPoints.push_back(foundPoint);
                        gribDataListLock.unlock();
                    } else {
                        sfcValues.push_back(foundPoint);
                    }
                }

                // Set values
                if (isIsobaricMsg) {
                    if (shortName == "u") {
                        foundPoint->setUMpers(value);
                    } else if (shortName == "v") {
                        foundPoint->setVMpers(value);
                    } else if (shortName == "t") {
                        foundPoint->setTempK(value);
                    } else if (shortName == "gh") {
                        foundPoint->setGeoPotentialHeight(value);
                    } else if (shortName == "rh") {
                        foundPoint->setRelHumidity(value);
                    }
                } else {
                    foundPoint->setSfcPressHPa(value / 100.0);
                }
            }

            /* At the end the iterator is deleted to free memory. */
            codes_grib_iterator_delete(iter);
        }

        /* At the end the codes_handle is deleted to free memory. */
        codes_handle_delete(h);
    }

    // Close file
    fclose(file);

    // Add Surface pressures back in
    gribDataListLock.lock();
    for (const std::shared_ptr<GribDataPoint> &point : dataPoints) {
        for (const std::shared_ptr<GribDataPoint> &sfc : sfcValues) {
            if (sfc->getLongitude() == point->getLongitude() && sfc->getLatitude() == point->getLatitude()) {
                point->setSfcPressHPa(sfc->getSfcPressHPa());
            }
        }
    }
    gribDataListLock.unlock();
}

void GribTile::downloadTile() {
    if (!downloaded) {
        string gribFileName = getGribFileName();
        remove(gribFileName.c_str());

        // Generate URL
        string url = getDownloadUrl();

        // Download file (Windows & Unix)
#ifdef _WIN32
        HRESULT hr = URLDownloadToFile(NULL, url.c_str(), gribFileName.c_str(), 0, NULL);

        if (!SUCCEEDED(hr)) {
            downloaded = false;
            return;
        }
#endif

#ifdef __unix__
        CURL *curl;
        FILE *file;
        CURLcode res;
        curl = curl_easy_init();
        if (curl){
            file = fopen(gribFileName.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            res = curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
        }
#endif

        // Extract Data
        extractData();

        // Set Downloaded flag
        downloaded = true;
    }
}

GribTile::GribTile(double lat, double lon, ptime dateTime) {
    downloaded = false;

    // Create tile bounds
    leftLon = max((short) floor(lon), (short) -180);
    rightLon = min((short) ceil(lon), (short) 180);
    bottomLat = max((short) floor(lat), (short) -90);
    topLat = min((short) ceil(lat), (short) 90);

    // Set date
    forecastDateUtc = dateTime;

    // Download asynchronously
    thread([this]() {
        downloadTile();
    }).detach();
}

short GribTile::getTopLat() const{
    return topLat;
}

short GribTile::getBottomLat() const {
    return bottomLat;
}

short GribTile::getLeftLon() const {
    return leftLon;
}

short GribTile::getRightLon() const {
    return rightLon;
}

ptime GribTile::getForecastDateUtc() const {
    return forecastDateUtc;
}

string GribTile::getGribFileName() const {
    stringstream ss;
    ss << "GribTile_" << getGribDateString()
       << "_t" << getCycleString() << "z"
       << "_f" << getForecastHourString()
       << "_l" << leftLon << "_t" << topLat
       << "_r" << rightLon << "_b" << bottomLat
       << ".grb";

    return ss.str();
}

std::shared_ptr<const GribDataPoint> GribTile::getClosestPoint(const GeoPoint &acftPos) {
    double minDist = -1;
    std::shared_ptr<GribDataPoint> pt(nullptr);

    gribDataListLock.lock();
    for (const std::shared_ptr<GribDataPoint> &point : dataPoints) {
        double dist = point->getDistanceFrom(acftPos);
        if (pt == nullptr || dist < minDist) {
            pt = point;
            minDist = dist;
        }
    }
    gribDataListLock.unlock();

    return pt;
}

bool GribTile::isValid(const ptime &dateTime) const{
    time_duration td = getForecastDateUtc() - dateTime;
    time_duration tdC = hours(1);
    return td.abs().hours() < tdC.hours();
}

bool GribTile::isAcftInside(const GeoPoint &pos) const{
    return pos.getLat() >= bottomLat && pos.getLat() <= topLat
           && pos.getLon() >= leftLon && pos.getLon() <= rightLon;
}

bool GribTile::equals(const GribTile &o) const {
    return leftLon == o.getLeftLon() && rightLon == o.getRightLon()
           && bottomLat == o.getBottomLat() && topLat == o.getTopLat()
           && isValid(o.getForecastDateUtc());
}

GribTile::~GribTile() {
    if (downloaded) {
        string gribFileName = getGribFileName();
        remove(gribFileName.c_str());
    }
}

string GribTile::getDownloadUrl() const{
    // Generate URL
    stringstream ss;
    string cycleStr = getCycleString();
    ss << "https://nomads.ncep.noaa.gov/cgi-bin/filter_gfs_0p25.pl?file=gfs.t"
       << cycleStr << "z.pgrb2.0p25.f" << getForecastHourString()
       << "&lev_1000_mb=on&lev_100_mb=on&lev_150_mb=on&lev_200_mb=on&lev_250_mb=on&lev_300_mb=on"
       << "&lev_350_mb=on&lev_400_mb=on&lev_450_mb=on&lev_500_mb=on&lev_550_mb=on&lev_600_mb=on"
       << "&lev_650_mb=on&lev_700_mb=on&lev_750_mb=on&lev_800_mb=on&lev_850_mb=on&lev_900_mb=on"
       << "&lev_925_mb=on&lev_950_mb=on&lev_975_mb=on&lev_mean_sea_level=on&lev_surface=on&var_HGT=on"
       << "&var_PRES=on&var_TMP=on&var_UGRD=on&var_VGRD=on&var_PRMSL=on&subregion="
       << "&leftlon=" << leftLon << "&rightlon=" << rightLon
       << "&toplat=" << topLat << "&bottomlat=" << bottomLat
       << "&dir=%2Fgfs." << getGribDateString() << "%2F" << cycleStr << "%2Fatmos";

    return ss.str();
}
