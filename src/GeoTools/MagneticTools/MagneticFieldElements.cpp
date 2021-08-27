//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "MagneticFieldElements.h"

using namespace AviationCalcUtil::GeoTools::MagneticTools;

MagneticFieldElements::MagneticFieldElements() {
    x = 0;
    y = 0;
    z = 0;
    h = 0;
    f = 0;
    incl = 0;
    decl = 0;
}

double MagneticFieldElements::getX() const {
    return x;
}

void MagneticFieldElements::setX(double newX) {
    x = newX;
}

double MagneticFieldElements::getY() const {
    return y;
}

void MagneticFieldElements::setY(double newY) {
    y = newY;
}

double MagneticFieldElements::getZ() const {
    return z;
}

void MagneticFieldElements::setZ(double newZ) {
    z = newZ;
}

double MagneticFieldElements::getH() const {
    return h;
}

void MagneticFieldElements::setH(double newH) {
    h = newH;
}

double MagneticFieldElements::getF() const {
    return f;
}

void MagneticFieldElements::setF(double newF) {
    f = newF;
}

double MagneticFieldElements::getDecl() const {
    return decl;
}

void MagneticFieldElements::setDecl(double newDecl) {
    decl = newDecl;
}

double MagneticFieldElements::getIncl() const {
    return incl;
}

void MagneticFieldElements::setIncl(double newIncl) {
    incl = newIncl;
}
