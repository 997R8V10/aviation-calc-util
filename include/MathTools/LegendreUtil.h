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
#include "MathTools/Polynomial.h"
#include "aviationcalc_exports.h"

using namespace std;

namespace AviationCalcUtil::MathTools {
    class LegendreUtil {
#ifdef _LIBRARY
    private:
        inline static map<tuple<int, int>, shared_ptr<Polynomial>> legendreCache;
        static mutex legendreCacheLock;
#endif
    public:
        AVIATIONCALC_EXPORT static shared_ptr<const Polynomial> legendrePolynomial(int n);
        AVIATIONCALC_EXPORT static double factorialRatio(int n, int m);
        AVIATIONCALC_EXPORT static double legendreFunction(int n, int m, double x);
    };
}


using namespace AviationCalcUtil::MathTools;
extern "C"
{
    extern AVIATIONCALC_EXPORT Polynomial *LegendreUtilPolynomial(int n);
    extern AVIATIONCALC_EXPORT double LegendreUtilFactorialRatio(int n, int m);
    extern AVIATIONCALC_EXPORT double LegendreUtilLegendreFunction(int n, int m, double x);


}

#endif //AVIATION_CALC_UTIL_LEGENDREUTIL_H
