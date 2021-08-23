//
// Created by pshivaraman on 2021-08-23.
//

#include "Polynomial.h"

AviationCalcUtil::MathTools::Polynomial::Polynomial(vector<double> *coefficients) {
    this->coefficients = coefficients;
}

vector<double> *AviationCalcUtil::MathTools::Polynomial::getCoefficients() {
    return new vector<double>(*coefficients);
}

double AviationCalcUtil::MathTools::Polynomial::evaluate(double x) {
    double y = 0;
    for (int i = 0; i < coefficients->size(); i++){
        y += coefficients->at(i) * pow(x, i);
    }

    return y;
}

AviationCalcUtil::MathTools::Polynomial *AviationCalcUtil::MathTools::Polynomial::derivative(int n) {
    if (n == 1){
        auto *dCoeffs = new vector<double>(coefficients->size() - 1);

        for (int i = 1; i < coefficients->size(); i++){
            dCoeffs->at(i - 1) = i * coefficients->at(i);
        }

        return new Polynomial(dCoeffs);
    }

    auto *q = new Polynomial(coefficients);

    for (int i = 0; i < n; i++){
        q = q->derivative(1);
    }

    return q;
}
