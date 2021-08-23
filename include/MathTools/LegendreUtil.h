//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_LEGENDREUTIL_H
#define AVIATION_CALC_UTIL_LEGENDREUTIL_H

#include <map>
#include <tuple>
#include <mutex>
#include "Polynomial.h"
#include "MathUtil.h"

using namespace std;

namespace AviationCalcUtil::MathTools {
    class LegendreUtil {
    private:
        static map<tuple<int, int>, Polynomial *> *legendreCache;
        static mutex legendreCacheLock;

    public:
        static Polynomial *legendrePolynomial(int n);
        static double factorialRatio(int n, int m);
        static double legendreFunction(int n, int m, double x);
    };
}

#endif //AVIATION_CALC_UTIL_LEGENDREUTIL_H
