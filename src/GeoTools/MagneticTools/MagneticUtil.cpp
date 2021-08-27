//
// Created by Prithvisagar Shivaraman on 8/24/21.
//
#define _LIBRARY
#include "MagneticUtil.h"
#include "MathUtil.h"
#include "GeoUtil.h"
#include <fstream>
#include <string>
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

    // Initial Values
    double epochDate = 0;
    std::string modelName;
    boost::posix_time::ptime timeUTC = boost::posix_time::second_clock::universal_time();
    boost::gregorian::date releaseDate(timeUTC.date());
    std::vector<std::shared_ptr<MagneticModelCoefficients>> coefficients;

    // Loop through file
    std::string line;
    int i = 0;
    while (std::getline(cofFile, line)){
        std::stringstream ss(line);
        if (i == 0){
            std::string dfStr("%m/%d/%Y");
            boost::gregorian::date_input_facet df(dfStr);

            ss.imbue(std::locale(ss.getloc(), &df));
            ss >> epochDate >> modelName >> releaseDate;
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

    if (i < 4){
        throw std::invalid_argument("WMM.COF file is invalid!");
    }

    modelLock.lock();
    model = std::make_shared<MagneticModel>(epochDate, modelName, releaseDate, coefficients);
    modelLock.unlock();
}

std::unique_ptr<MagneticResult> MagneticUtil::getMagneticField(const GeoPoint &point, const boost::gregorian::date &date) {
    modelLock.lock();
    if (model == nullptr){
        try {
            loadData();
        } catch (const std::exception&){}
    }
    modelLock.unlock();

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
