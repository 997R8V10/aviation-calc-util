//
// Created by Prithvisagar Shivaraman on 8/24/21.
//
#define _LIBRARY
#include "GeoTools/MagneticTools/MagneticUtil.h"
#include "MathTools/MathUtil.h"
#include "GeoTools/GeoUtil.h"
#include <fstream>
#include <string>
#include <iostream>
#include <boost/date_time.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::MathTools;


std::mutex MagneticUtil::modelLock;

void MagneticUtil::loadData() {
    if (model != nullptr){
        return;
    }

    std::string filename("magnetic/WMM.COF");

    // Read file
    std::ifstream cofFile(filename);

    if (!cofFile.is_open()){
        throw std::invalid_argument("Error Opening Magnetic File: WMM.COF!");
    }

    cofFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);

    // Initial Values
    double epochDate = 0;
    std::string modelName;
    boost::posix_time::ptime timeUTC = boost::posix_time::second_clock::universal_time();
    boost::gregorian::date releaseDate(timeUTC.date());
    std::vector<std::shared_ptr<MagneticModelCoefficients>> coefficients;

    // Loop through file
    std::string line;
    int i = 0;
    try{
        while (std::getline(cofFile, line)){
            std::stringstream ss(line);

            if (i == 0){
                std::string strDate;

                // Read line
                ss >> epochDate >> modelName >> strDate;

                // Get Release Date
                const std::locale loc = std::locale(std::locale::classic(), new boost::gregorian::date_input_facet("%m/%d/%Y"));
                std::istringstream is(strDate);
                is.imbue(loc);

                is >> releaseDate;
            } else {
                int n, m;
                double g, h, g_dot, h_dot;
                ss >> n >> m >> g >> h >> g_dot >> h_dot;

                if (n > 9999){
                    // Assume end of file
                    break;
                }
                coefficients.push_back(std::make_shared<MagneticModelCoefficients>(n, m, g, h, g_dot, h_dot));
            }
            i++;
        }
    } catch (const std::ifstream::failure &ex){
        if (cofFile.bad() || cofFile.fail()){
            cofFile.close();
            throw std::invalid_argument("WMM.COF file is invalid!");
        }
    }

    cofFile.close();

    if (i < 4){
        throw std::invalid_argument("WMM.COF file is invalid!");
    }

    const std::lock_guard<std::mutex> gd_lock(modelLock);
    model = std::make_shared<MagneticModel>(epochDate, modelName, releaseDate, coefficients);
}

std::unique_ptr<MagneticResult> MagneticUtil::getMagneticField(const GeoPoint &point, const boost::gregorian::date &date) {
    const std::lock_guard<std::mutex> gd_lock(modelLock);
    if (model == nullptr){
        try {
            loadData();
        } catch (const std::exception&){}
    }

    return std::make_unique<MagneticResult>(model, point, date);
}

double MagneticUtil::getEpochYear(const boost::gregorian::date &date) {
    double year = date.year();
    double totalDays = (boost::gregorian::date(date.year() + 1, 1, 1) - boost::gregorian::date(date.year(), 1, 1)).days();
    year += (date.day_of_year() - 1) / totalDays;

    return year;
}

void MagneticUtil::getSpherical(const GeoPoint &point, double &lambda, double &phiPrime, double &r) {
    double phi = MathUtil::convertDegreesToRadians(point.getLat());
    lambda = MathUtil::convertDegreesToRadians(point.getLon());
    double h = MathUtil::convertFeetToMeters(point.getAlt());

    double sinPhi = std::sin(phi);
    double cosPhi = std::cos(phi);

    // Calculate Rc
    double f = 1 / EARTH_WGS84_RECIPROCAL_FLATTENING;
    double e2 = f * (2 - f);
    double Rc = EARTH_WGS84_SEMI_MAJOR_AXIS / std::sqrt(1 - e2 * std::pow(sinPhi, 2));

    double p = (Rc + h) * cosPhi;
    double z = (Rc * (1 - e2) + h) * sinPhi;
    r = std::sqrt(std::pow(p, 2) + std::pow(z, 2));
    phiPrime = std::asin(z / r);
}

double MagneticUtil::convertMagneticToTrue(double magneticBearing, double declination) {
    return GeoUtil::normalizeHeading(magneticBearing + declination);
}

double MagneticUtil::convertMagneticToTrue(double magneticBearing, const GeoPoint &position) {
    boost::posix_time::ptime timeUTC = boost::posix_time::second_clock::universal_time();
    std::unique_ptr<MagneticResult> m = getMagneticField(position, timeUTC.date());
    double declin = m->getMainFieldElements().getDecl();
    declin = static_cast<double>(static_cast<int>(declin * 10)) / 10.0;

    return convertMagneticToTrue(magneticBearing, declin);
}

double MagneticUtil::convertTrueToMagnetic(double trueBearing, double declination) {
    return GeoUtil::normalizeHeading(trueBearing - declination);
}

double MagneticUtil::convertTrueToMagnetic(double trueBearing, const GeoPoint &position) {
    boost::posix_time::ptime timeUTC = boost::posix_time::second_clock::universal_time();
    std::unique_ptr<MagneticResult> m = getMagneticField(position, timeUTC.date());
    double declin = m->getMainFieldElements().getDecl();
    declin = static_cast<double>(static_cast<int>(declin * 10)) / 10.0;

    return convertTrueToMagnetic(trueBearing, declin);
}
