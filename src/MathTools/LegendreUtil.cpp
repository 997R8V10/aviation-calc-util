//
// Created by pshivaraman on 2021-08-23.
//
#define _LIBRARY
#include "LegendreUtil.h"
#include "MathUtil.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace AviationCalcUtil::MathTools;

std::mutex LegendreUtil::legendreCacheLock;

std::shared_ptr<const Polynomial> LegendreUtil::legendrePolynomial(int n) {
    vector<double> coeffs(n + 1);

    for (int  m = 0; m <= n / 2; m++){
        coeffs.at(n - 2 * m) = pow(-1, m) / pow(2, n);
        coeffs.at(n - 2 * m) *= factorialRatio(2 * n - 2 * m, n - m) / (double)(MathUtil::factorial(m) * MathUtil::factorial(n - 2 * m));
    }

    return std::make_shared<Polynomial>(coeffs);
}

double LegendreUtil::factorialRatio(int n, int m) {
    if (n > m){
        return n * factorialRatio(n - 1, m);
    }

    return 1;
}

double LegendreUtil::legendreFunction(int n, int m, double x) {
    std::shared_ptr<Polynomial> p;
    tuple<int, int> key = make_tuple(n, m);

    legendreCacheLock.lock();
    try {
        p = legendreCache.at(key);
    } catch (const std::out_of_range&){
        p = legendrePolynomial(n)->derivative(m);
        legendreCache.at(key) = p;
    }
    legendreCacheLock.unlock();

    return pow(1 - pow(x, 2), m / 2.0) * p->evaluate(x);
}
