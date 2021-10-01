//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_POLYNOMIAL_H
#define AVIATION_CALC_UTIL_POLYNOMIAL_H
#include <vector>
#include <memory>
#include "aviationcalc_exports.h"

using namespace std;

namespace AviationCalcUtil::MathTools {
    class Polynomial {
    private:
        vector<double> coefficients;

    public:
        AVIATIONCALC_EXPORT explicit Polynomial(const vector<double> &coefficients);

        AVIATIONCALC_EXPORT Polynomial(const Polynomial &other);

        AVIATIONCALC_EXPORT double evaluate(double x) const;

        AVIATIONCALC_EXPORT shared_ptr<Polynomial> derivative(int n) const;

        AVIATIONCALC_EXPORT const vector<double> &getCoefficients() const;
    };
}

#endif //AVIATION_CALC_UTIL_POLYNOMIAL_H
