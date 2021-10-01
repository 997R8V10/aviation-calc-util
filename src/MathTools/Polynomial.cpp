//
// Created by pshivaraman on 2021-08-23.
//

#include "MathTools/Polynomial.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

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

Polynomial::Polynomial(const Polynomial &other) {
    coefficients = other.getCoefficients();
}
/*
Polynomial *CreatePolynomial(double *coefficients, int coefficientsSize) {
    vector<double> coefficientsVector{};
    for (int i = 0; i < coefficientsSize; i++)
    {
        coefficientsVector.push_back(coefficients[i]);
        cout << coefficients[i];
    }
    cout << endl;

    return new Polynomial(coefficientsVector);
}

void DisposePolynomial(Polynomial *ptr) {
    if (ptr != NULL)
    {
        delete ptr;
        ptr = NULL;
    }
}

double PolynomialEvaluate(Polynomial *ptr, double x) {
    if (ptr != NULL)
    {
        return ptr->evaluate(x);
    }
    return -1;
}

Polynomial *PolynomialDerivative(Polynomial *ptr, int n) {
    if (ptr != NULL)
    {
        auto toReturn = ptr->derivative(n);
        if (toReturn != NULL)
        {
            return new Polynomial(*toReturn);
        }
    }
    return new Polynomial(std::vector<double>{});
}

double *PolynomialGetCoefficients(Polynomial* ptr, int& size) {
    if (ptr != nullptr) {
        auto coeffsVector = ptr->getCoefficients();
        int s = static_cast<int>(coeffsVector.size());
        auto *coeffsArray = new double[s];
        for (int i = 0; i < s; i++) {
            coeffsArray[i] = coeffsVector[i];
        }
        size = s;
        return coeffsArray;
    }
    size = 0;
    return new double[0];
}
*/