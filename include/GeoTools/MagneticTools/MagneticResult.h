//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICRESULT_H
#define AVIATION_CALC_UTIL_MAGNETICRESULT_H

#include "MagneticFieldElements.h"
#include "MagneticModel.h"
#include "GeoPoint.h"
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>

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
        MagneticFieldElements fieldElements;
        MagneticFieldElements secElements;

        void calculateMagneticField();

        void rotateMagneticField();

        void calculateDerivedValues();

    public:
        MagneticResult(const std::shared_ptr<const MagneticModel> &model, const GeoPoint &point,
                       boost::gregorian::date date);

        const MagneticFieldElements &getMainFieldElements() const;

        const MagneticFieldElements &getSecularFieldElements() const;
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICRESULT_H
