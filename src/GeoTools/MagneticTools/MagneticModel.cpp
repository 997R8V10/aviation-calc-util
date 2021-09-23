//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "GeoTools/MagneticTools/MagneticModel.h"
#include "GeoTools/MagneticTools/MagneticUtil.h"
#include "InteropTools/InteropUtil.h"

using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace std;
using namespace boost::gregorian;

MagneticModel::MagneticModel(double modelEpoch, const std::string &modelName, const date &releaseDate,
                             const std::vector<shared_ptr<MagneticModelCoefficients>> &coefficients) {
    this->modelEpoch = modelEpoch;
    this->modelName = modelName;
    this->releaseDate = date(releaseDate);

    // Initialize vector
    for (int n = 0; n <= MagneticUtil::WMM_EXPANSION; n++) {
        auto coeffRow = vector<shared_ptr<MagneticModelCoefficients>>();
        for (int m = 0; m <= MagneticUtil::WMM_EXPANSION; m++) {
            coeffRow.push_back(make_shared<MagneticModelCoefficients>(n, m, 0, 0, 0, 0));
        }
        this->coeffs.push_back(coeffRow);
    }

    for (const shared_ptr<MagneticModelCoefficients> &coeff : coefficients) {
        if (coeff->getN() <= MagneticUtil::WMM_EXPANSION && coeff->getM() <= MagneticUtil::WMM_EXPANSION) {
            this->coeffs[coeff->getN()][coeff->getM()] = coeff;
        }
    }
}

shared_ptr<const MagneticModelCoefficients> MagneticModel::getCoeffs(int n, int m) const {
    if (n <= MagneticUtil::WMM_EXPANSION && m <= MagneticUtil::WMM_EXPANSION) {
        return this->coeffs[n][m];
    }
    return make_shared<MagneticModelCoefficients>(n, m, 0, 0, 0, 0);
}

double MagneticModel::getModelEpoch() const {
    return modelEpoch;
}

const std::string &MagneticModel::getModelName() const {
    return modelName;
}

const date &MagneticModel::getReleaseDate() const {
    return releaseDate;
}
/*
std::shared_ptr<const MagneticModel> *
CreateMagModel(double modelEpoch, const char *modelName, InteropDateStruct dStruct,
               MagneticModelCoefficients **coefficients, int coeffsSize) {
    if (coefficients == NULL || modelName == NULL) {
        return NULL;
    }

    // Copy coefficients
    std::vector<shared_ptr<MagneticModelCoefficients>> coeffs{};

    for (int i = 0; i < coeffsSize; i++) {
        MagneticModelCoefficients *coeffElem = *(coefficients + (i * sizeof(*coefficients)));
        coeffs.push_back(std::make_shared<MagneticModelCoefficients>(*coeffElem));
    }

    return new std::shared_ptr<const MagneticModel>(
            std::make_shared<const MagneticModel>(
                    modelEpoch,
                    std::string(modelName),
                    InteropStructToBoostDate(dStruct),
                    coeffs
            )
    );
}

void DisposeMagModel(std::shared_ptr<const MagneticModel> *model) {
    if (model != NULL){
        delete model;
        model = NULL;
    }
}

MagneticModelCoefficients *MagModelGetCoeffs(std::shared_ptr<const MagneticModel> *model, int n, int m) {
    if (model == NULL){
        return NULL;
    }

    auto coeffs = model->get()->getCoeffs(n, m);

    if (coeffs == nullptr){
        return nullptr;
    }

    return new MagneticModelCoefficients(*coeffs);
}

double MagModelGetModelEpoch(std::shared_ptr<const MagneticModel> *model) {
    if (model == NULL){
        return -1;
    }

    return model->get()->getModelEpoch();
}

const char *MagModelGetModelName(std::shared_ptr<const MagneticModel> *model) {
    if (model == NULL){
        return "";
    }

    return InteropCppStrToCStr(model->get()->getModelName());
}

InteropDateStruct MagModelGetReleaseDate(std::shared_ptr<const MagneticModel> *model) {
    if (model == NULL){
        return InteropDateStruct{0, 0, 0};
    }

    return InteropBoostDateToStruct(model->get()->getReleaseDate());
}
*/