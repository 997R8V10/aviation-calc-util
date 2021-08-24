//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICMODEL_H
#define AVIATION_CALC_UTIL_MAGNETICMODEL_H

#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <vector>
#include "MagneticModelCoefficients.h"

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticModel {
    private:
        double modelEpoch;
        std::string modelName;
        boost::gregorian::date releaseDate;
        std::vector<std::vector<MagneticModelCoefficients *>> coeffs;

    public:
        MagneticModel(double modelEpoch, std::string modelName, boost::gregorian::date releaseDate, std::vector<MagneticModelCoefficients *> *coefficients);

        MagneticModelCoefficients *getCoeffs(int n, int m);
        double getModelEpoch();
        std::string getModelName();
        boost::gregorian::date getReleaseDate();
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICMODEL_H
