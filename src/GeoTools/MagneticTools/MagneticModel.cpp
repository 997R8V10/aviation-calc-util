//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "GeoTools/MagneticTools/MagneticModel.h"
#include "GeoTools/MagneticTools/MagneticUtil.h"

using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace std;
using namespace boost::gregorian;

MagneticModel::MagneticModel(double modelEpoch, const std::string &modelName, const date &releaseDate,
                             const std::vector<shared_ptr<MagneticModelCoefficients>> &coefficients) {
    this->modelEpoch = modelEpoch;
    this->modelName = modelName;
    this->releaseDate = date(releaseDate);
    vector<vector<shared_ptr<MagneticModelCoefficients>>> newCoeffs(MagneticUtil::WMM_EXPANSION + 1);

    // Initialize vector
    int n = 0;
    for (auto & coeff : this->coeffs){
        int m = 0;
        coeff = vector<shared_ptr<MagneticModelCoefficients>>(MagneticUtil::WMM_EXPANSION + 1);
        for (auto & subCoeff: coeff){
            subCoeff = make_shared<MagneticModelCoefficients>(n, m, 0, 0, 0, 0);
            m++;
        }
        n++;
    }

    for (const shared_ptr<MagneticModelCoefficients> &coeff : coefficients){
        if (coeff->getN() <= MagneticUtil::WMM_EXPANSION && coeff->getM() <= MagneticUtil::WMM_EXPANSION)
        {
            this->coeffs[coeff->getN()][coeff->getM()] = coeff;
        }
    }

    coeffs = move(newCoeffs);
}

shared_ptr<const MagneticModelCoefficients> MagneticModel::getCoeffs(int n, int m) const {
    return this->coeffs[n][m];
}

double MagneticModel::getModelEpoch() const {
    return modelEpoch;
}

const std::string &MagneticModel::getModelName() const{
    return modelName;
}

const date &MagneticModel::getReleaseDate() const {
    return releaseDate;
}
