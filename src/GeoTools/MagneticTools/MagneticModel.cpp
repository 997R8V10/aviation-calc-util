//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "MagneticModel.h"
#include "MagneticUtil.h"

using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace std;
using namespace boost::gregorian;

MagneticModel::MagneticModel(double modelEpoch, std::string modelName, boost::gregorian::date releaseDate,
                             std::vector<MagneticModelCoefficients *> *coefficients) {
    this->modelEpoch = modelEpoch;
    this->modelName = modelName;
    this->releaseDate = date(releaseDate);
    this->coeffs = vector<vector<MagneticModelCoefficients *>>(MagneticUtil::WMM_EXPANSION + 1);

    for (int i = 0; i < this->coeffs.size(); i++){
        this->coeffs[i] = vector<MagneticModelCoefficients *>(MagneticUtil::WMM_EXPANSION + 1);
    }

    for (MagneticModelCoefficients *coeff : *coefficients){
        if (coeff->getN() <= MagneticUtil::WMM_EXPANSION && coeff->getM() <= MagneticUtil::WMM_EXPANSION)
        {
            this->coeffs[coeff->getN()][coeff->getM()] = coeff->getCopy();
        }
    }
}

MagneticModelCoefficients *MagneticModel::getCoeffs(int n, int m) {
    return this->coeffs[n][m];
}

double MagneticModel::getModelEpoch() {
    return modelEpoch;
}

std::string MagneticModel::getModelName() {
    return modelName;
}

boost::gregorian::date MagneticModel::getReleaseDate() {
    return releaseDate;
}
