//
// Created by pshivaraman on 2021-08-23.
//

#include "LegendreUtil.h"
using namespace AviationCalcUtil::MathTools;

map<tuple<int, int>, Polynomial *> *LegendreUtil::legendreCache = new map<tuple<int, int>, Polynomial *>();
mutex LegendreUtil::legendreCacheLock;

Polynomial *LegendreUtil::legendrePolynomial(int n) {
    auto *coeffs = new vector<double>(n + 1);

    for (int  m = 0; m <= n / 2; m++){
        coeffs->at(n - 2 * m) = pow(-1, m) / pow(2, n);
        coeffs->at(n - 2 * m) *= factorialRatio(2 * n - 2 * m, n - m) / (MathUtil::factorial(m) * MathUtil::factorial(n - 2 * m));
    }

    return new Polynomial(coeffs);
}

double LegendreUtil::factorialRatio(int n, int m) {
    if (n > m){
        return n * factorialRatio(n - 1, m);
    }

    return 1;
}

double LegendreUtil::legendreFunction(int n, int m, double x) {
    Polynomial *p;
    tuple<int, int> key = make_tuple(n, m);

    legendreCacheLock.lock();
    try {
        p = legendreCache->at(key);
    } catch (const std::out_of_range&){
        p = legendrePolynomial(n)->derivative(m);
        legendreCache->at(key) = p;
    }
    legendreCacheLock.unlock();

    return pow(1 - pow(x, 2), m / 2.0) * p->evaluate(x);
}
