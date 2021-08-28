//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICMODEL_H
#define AVIATION_CALC_UTIL_MAGNETICMODEL_H

#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <vector>
#include "GeoTools/MagneticTools/MagneticModelCoefficients.h"
#include "aviationcalc_exports.h"

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticModel {
    private:
        double modelEpoch;
        std::string modelName;
        boost::gregorian::date releaseDate;
        std::vector<std::vector<std::shared_ptr<MagneticModelCoefficients>>> coeffs;

    public:
        AVIATIONCALC_EXPORT MagneticModel(double modelEpoch, const std::string &modelName, const boost::gregorian::date &releaseDate,
                      const std::vector<std::shared_ptr<MagneticModelCoefficients>> &coefficients);

        AVIATIONCALC_EXPORT std::shared_ptr<const MagneticModelCoefficients> getCoeffs(int n, int m) const;
        AVIATIONCALC_EXPORT double getModelEpoch() const;
        AVIATIONCALC_EXPORT const std::string &getModelName() const;
        AVIATIONCALC_EXPORT const boost::gregorian::date &getReleaseDate() const;
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICMODEL_H
