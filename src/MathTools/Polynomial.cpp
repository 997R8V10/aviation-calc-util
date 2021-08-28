//
// Created by pshivaraman on 2021-08-23.
//

#include "MathTools/Polynomial.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace AviationCalcUtil::MathTools;

Polynomial::Polynomial(const vector<double> &coefficients) {
    this->coefficients = coefficients;
}

const vector<double> &Polynomial::getCoefficients() const {
    return coefficients;
}

double Polynomial::evaluate(double x) const{
    double y = 0;
    for (int i = 0; i < coefficients.size(); i++){
        y += coefficients.at(i) * pow(x, i);
    }

    return y;
}

std::shared_ptr<Polynomial> AviationCalcUtil::MathTools::Polynomial::derivative(int n) const {
    if (n == 1){
        std::vector<double> dCoeffs(coefficients.size() - 1);

        for (int i = 1; i < coefficients.size(); i++){
            dCoeffs.at(i - 1) = i * coefficients.at(i);
        }

        return std::make_shared<Polynomial>(dCoeffs);
    }

    auto q = std::make_shared<Polynomial>(coefficients);

    for (int i = 0; i < n; i++){
        q = q->derivative(1);
    }

    return q;
}
