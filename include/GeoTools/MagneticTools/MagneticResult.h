//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICRESULT_H
#define AVIATION_CALC_UTIL_MAGNETICRESULT_H

#include "GeoTools/MagneticTools/MagneticFieldElements.h"
#include "GeoTools/MagneticTools/MagneticModel.h"
#include "GeoTools/GeoPoint.h"
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "aviationcalc_exports.h"
#include "InteropTools/InteropUtil.h"

using namespace AviationCalcUtil::GeoTools;
namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticResult {
    private:
        double lambda;
        double phi;
        double phiPrime;
        double r;
        double t0;
        boost::gregorian::date t;
        std::shared_ptr<const MagneticModel> model;
        MagneticFieldElements fieldElements{};
        MagneticFieldElements secElements{};

        void calculateMagneticField();

        void rotateMagneticField();

        void calculateDerivedValues();

    public:
        AVIATIONCALC_EXPORT MagneticResult(const std::shared_ptr<const MagneticModel> &model, const GeoPoint &point,
                                           boost::gregorian::date date);

        AVIATIONCALC_EXPORT const MagneticFieldElements &getMainFieldElements() const;

        AVIATIONCALC_EXPORT const MagneticFieldElements &getSecularFieldElements() const;
    };
}

extern "C"
{
extern AVIATIONCALC_EXPORT std::shared_ptr<const MagneticResult> *CreateMagneticResult(std::shared_ptr<const MagneticModel> *model, GeoPoint *point, InteropDateStruct dStruct);
extern AVIATIONCALC_EXPORT void DisposeMagneticResult(std::shared_ptr<const MagneticResult> *ptr);
extern AVIATIONCALC_EXPORT MagneticFieldElements *MagneticResultGetMainFieldElements(std::shared_ptr<const MagneticResult> *ptr);
extern AVIATIONCALC_EXPORT MagneticFieldElements *MagneticResultGetSecularFieldElements(std::shared_ptr<const MagneticResult> *ptr);
}
#endif //AVIATION_CALC_UTIL_MAGNETICRESULT_H
