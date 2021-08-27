//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_LEGENDREUTIL_H
#define AVIATION_CALC_UTIL_LEGENDREUTIL_H

#ifdef _LIBRARY
#include <map>
#include <tuple>
#include <mutex>
#endif

#include <memory>
#include "Polynomial.h"
#include "aviationcalc_exports.h"

using namespace std;

namespace AviationCalcUtil::MathTools {
    class AVIATIONCALC_EXPORTS LegendreUtil {
#ifdef _LIBRARY
    private:
        inline static map<tuple<int, int>, shared_ptr<Polynomial>> legendreCache;
        static mutex legendreCacheLock;
#endif
    public:
        static shared_ptr<const Polynomial> legendrePolynomial(int n);
        static double factorialRatio(int n, int m);
        static double legendreFunction(int n, int m, double x);
    };
}

#endif //AVIATION_CALC_UTIL_LEGENDREUTIL_H
