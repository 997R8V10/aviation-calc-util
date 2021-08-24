//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICRESULT_H
#define AVIATION_CALC_UTIL_MAGNETICRESULT_H

#include "MagneticFieldElements.h"
#include "MagneticModel.h"
#include "GeoPoint.h"
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
        MagneticModel *model;
        MagneticFieldElements *fieldElements;
        MagneticFieldElements *secElements;

        void calculateMagneticField();

        void rotateMagneticField();

        void calculateDerivedValues();

    public:
        MagneticResult(MagneticModel *model, GeoPoint *point, boost::gregorian::date date);

        MagneticFieldElements *getMainFieldElements();

        MagneticFieldElements *getSecularFieldElements();
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICRESULT_H
