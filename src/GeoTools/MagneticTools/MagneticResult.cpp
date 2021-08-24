//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "MagneticResult.h"
#include "LegendreUtil.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <MathUtil.h>
#include "MagneticUtil.h"

using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace AviationCalcUtil::MathTools;

void MagneticResult::calculateMagneticField() {
    double sinPhi = std::sin(phiPrime);
    double cosPhi = std::cos(phiPrime);
    double g, h, dg, dh;

    // Step 3 in WMM Technical Report
    for (int n = 1; n < MagneticUtil::WMM_EXPANSION; n++)
    {
        double nn = n + 1;
        double f = std::pow(MagneticUtil::GEOMAGNETIC_REFERENCE_RADIUS / r, n + 2);

        for (int m = 0; m <= n; m++)
        {
            // Schmidt Semi-Normalized Value
            double p = LegendreUtil::legendreFunction(n, m, sinPhi);
            double q = LegendreUtil::legendreFunction(n + 1, m, sinPhi);

            if (m > 0)
            {
                p *= std::sqrt(2 / LegendreUtil::factorialRatio(n + m, n - m));
                q *= std::sqrt(2 / LegendreUtil::factorialRatio(n + m, n - m));
            }

            double dp = nn * std::tan(phiPrime) * p - (nn - m) / cosPhi * q; // Equation 16
            MagneticModelCoefficients *wmmCoefficients = model->getCoeffs(n, m)->getPointOnDate(t0, t); // Equation 9

            g = wmmCoefficients->getG();
            h = wmmCoefficients->getH();
            dg = wmmCoefficients->getGDot();
            dh = wmmCoefficients->getHDot();

            // Calculate field elements
            double sinMLambda = std::sin(m * lambda);
            double cosMLambda = std::cos(m * lambda);
            fieldElements->setX(fieldElements->getX() + (-f * (g * cosMLambda + h * sinMLambda) * dp)); // Equation 10
            fieldElements->setY(fieldElements->getY() + (f / cosPhi * m * (g * sinMLambda - h * cosMLambda) * p)); // Equation 11
            fieldElements->setZ(fieldElements->getZ() + (-nn * f * (g * cosMLambda + h * sinMLambda) * p)); // Equation 12
            secElements->setX(secElements->getX() + (-f * (dg * cosMLambda + dh * sinMLambda) * dp)); // Equation 13
            secElements->setY(secElements->getY() + (f / cosPhi * m * (dg * sinMLambda - dh * cosMLambda) * p)); // Equation 14
            secElements->setZ(secElements->getZ() + (-nn * f * (dg * cosMLambda + dh * sinMLambda) * p)); // Equation 15
        }
    }
}

void MagneticResult::rotateMagneticField() {
    // Equation 17
    fieldElements->setX(fieldElements->getX() * std::cos(phiPrime - phi) - fieldElements->getZ() * std::sin(phiPrime - phi));
    fieldElements->setZ(fieldElements->getX() * std::sin(phiPrime - phi) + fieldElements->getZ() * std::cos(phiPrime - phi));

    // Equation 18
    secElements->setX(secElements->getX() * std::cos(phiPrime - phi) - secElements->getZ() * std::sin(phiPrime - phi));
    secElements->setZ(secElements->getX() * std::sin(phiPrime - phi) + secElements->getZ() * std::cos(phiPrime - phi));
}

void MagneticResult::calculateDerivedValues() {
    // Equation 19
    fieldElements->setH(std::sqrt(std::pow(fieldElements->getX(), 2) + std::pow(fieldElements->getY(), 2)));
    fieldElements->setF(std::sqrt(std::pow(fieldElements->getH(), 2) + std::pow(fieldElements->getZ(), 2)));
    fieldElements->setIncl(MathUtil::convertRadiansToDegrees(std::atan2(fieldElements->getZ(), fieldElements->getH())));
    fieldElements->setDecl(MathUtil::convertRadiansToDegrees(std::atan2(fieldElements->getY(), fieldElements->getX())));

    // Equation 20
    secElements->setH((fieldElements->getX() * secElements->getX() + fieldElements->getY() * secElements->getY()) / fieldElements->getH());
    secElements->setF((fieldElements->getX() * secElements->getX() + fieldElements->getY() * secElements->getY() + fieldElements->getZ() * secElements->getZ()) / fieldElements->getF());
    secElements->setIncl((fieldElements->getH() * secElements->getZ() - fieldElements->getZ() * secElements->getH()) / std::pow(fieldElements->getF(), 2));
    secElements->setDecl((fieldElements->getX() * secElements->getY() - fieldElements->getY() * secElements->getX()) / std::pow(fieldElements->getH(), 2));
    secElements->setIncl(MathUtil::convertRadiansToDegrees(secElements->getIncl()));
    secElements->setDecl(MathUtil::convertRadiansToDegrees(secElements->getDecl()));
}

MagneticResult::MagneticResult(MagneticModel *model, GeoPoint *point, boost::gregorian::date date) {
    model = model;

    t0 = model->getModelEpoch();
    t = date;

    fieldElements = new MagneticFieldElements();
    secElements = new MagneticFieldElements();

    if (model == nullptr)
    {
        return;
    }

    // Convert to spherical
    // Equations 7-8
    MagneticUtil::getSpherical(point, lambda, phiPrime, r);
    phi = MathUtil::convertDegreesToRadians(point->getLat());

    // Calculate field values
    // Equations 9-16
    calculateMagneticField();

    // Equations 17-18
    rotateMagneticField();

    // Equations 19-20
    calculateDerivedValues();
}

MagneticFieldElements *MagneticResult::getMainFieldElements() {
    return fieldElements;
}

MagneticFieldElements *MagneticResult::getSecularFieldElements() {
    return secElements;
}
