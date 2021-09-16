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

using namespace AviationCalcUtil::MathTools;
extern "C"
{
    extern AVIATIONCALC_EXPORT Polynomial* CreatePolynomial(double* coefficients, int coefficientsSize);
    extern AVIATIONCALC_EXPORT void DisposePolynomial(Polynomial* ptr);
    extern AVIATIONCALC_EXPORT double PolynomialEvaluate(Polynomial* ptr, double x);
    extern AVIATIONCALC_EXPORT Polynomial* PolynomialDerivative(Polynomial* ptr, int n);
    extern AVIATIONCALC_EXPORT double* PolynomialGetCoefficients(Polynomial* ptr, int& size);
}

#endif //AVIATION_CALC_UTIL_POLYNOMIAL_H
