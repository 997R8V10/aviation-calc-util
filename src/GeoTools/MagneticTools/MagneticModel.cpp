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
    for (int n = 0; n <= MagneticUtil::WMM_EXPANSION; n++){
        auto coeffRow = vector<shared_ptr<MagneticModelCoefficients>>(MagneticUtil::WMM_EXPANSION + 1);
        for (int m = 0; m <= MagneticUtil::WMM_EXPANSION; m++){
            coeffRow.push_back(make_shared<MagneticModelCoefficients>(n, m, 0, 0, 0, 0));
        }
        newCoeffs.push_back(coeffRow);
    }

    for (const shared_ptr<MagneticModelCoefficients> &coeff : coefficients){
        if (coeff->getN() <= MagneticUtil::WMM_EXPANSION && coeff->getM() <= MagneticUtil::WMM_EXPANSION)
        {
            newCoeffs[coeff->getN()][coeff->getM()] = coeff;
        }
    }

    coeffs = move(newCoeffs);
}

shared_ptr<const MagneticModelCoefficients> MagneticModel::getCoeffs(int n, int m) const {
    if (n <= MagneticUtil::WMM_EXPANSION && m <= MagneticUtil::WMM_EXPANSION){
        return this->coeffs[n][m];
    }
    return make_shared<MagneticModelCoefficients>(n, m, 0, 0, 0, 0);
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
