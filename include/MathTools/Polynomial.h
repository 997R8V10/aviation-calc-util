//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_POLYNOMIAL_H
#define AVIATION_CALC_UTIL_POLYNOMIAL_H
#include <vector>

using namespace std;

namespace AviationCalcUtil::MathTools {
    class Polynomial {
    private:
        vector<double> *coefficients;

    public:
        explicit Polynomial(vector<double> *coefficients);

        double evaluate(double x);

        Polynomial *derivative(int n);

        vector<double> *getCoefficients();
    };
}


#endif //AVIATION_CALC_UTIL_POLYNOMIAL_H
