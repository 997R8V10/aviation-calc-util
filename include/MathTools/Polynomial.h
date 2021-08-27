//
// Created by pshivaraman on 2021-08-23.
//

#ifndef AVIATION_CALC_UTIL_POLYNOMIAL_H
#define AVIATION_CALC_UTIL_POLYNOMIAL_H
#include <vector>
#include <memory>

using namespace std;

namespace AviationCalcUtil::MathTools {
    class Polynomial {
    private:
        vector<double> coefficients;

    public:
        explicit Polynomial(const vector<double> &coefficients);

        double evaluate(double x) const;

        shared_ptr<Polynomial> derivative(int n) const;

        const vector<double> &getCoefficients() const;
    };
}


#endif //AVIATION_CALC_UTIL_POLYNOMIAL_H
