//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#include "GeoTools/MagneticTools/MagneticFieldElements.h"

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

MagneticFieldElements::MagneticFieldElements(const MagneticFieldElements &other) {
    x = other.getX();
    y = other.getY();
    z = other.getZ();
    h = other.getH();
    f = other.getF();
    incl = other.getIncl();
    decl = other.getDecl();
}
/*
MagneticFieldElements *CreateMagneticFieldElements() {
    return new MagneticFieldElements();
}

double MagneticFieldElementsGetX(MagneticFieldElements *ptr) {
    if (ptr != nullptr)
    {
        return ptr->getX();
    }
    return -1;
}

void MagneticFieldElementsSetX(MagneticFieldElements *ptr, double newX) {
    if (ptr != nullptr)
    {
        ptr->setX(newX);
    }
}

double MagneticFieldElementsGetY(MagneticFieldElements *ptr) {
    if (ptr != nullptr)
    {
        return ptr->getY();
    }
    return -1;
}

void MagneticFieldElementsSetY(MagneticFieldElements *ptr, double newY) {
    if (ptr != nullptr)
    {
        ptr->setY(newY);
    }
}

double MagneticFieldElementsGetZ(MagneticFieldElements *ptr) {
    if (ptr != nullptr)
    {
        return ptr->getZ();
    }
    return -1;
}

void MagneticFieldElementsSetZ(MagneticFieldElements *ptr, double newZ) {
    if (ptr != nullptr)
    {
        ptr->setZ(newZ);
    }
}

double MagneticFieldElementsGetH(MagneticFieldElements *ptr) {
    if (ptr != nullptr)
    {
        return ptr->getH();
    }
    return -1;
}

void MagneticFieldElementsSetH(MagneticFieldElements *ptr, double newH) {
    if (ptr != nullptr)
    {
        ptr->setH(newH);
    }
}

double MagneticFieldElementsGetF(MagneticFieldElements *ptr) {
    if (ptr != nullptr)
    {
        return ptr->getF();
    }
    return -1;
}

void MagneticFieldElementsSetF(MagneticFieldElements *ptr, double newF) {
    if (ptr != nullptr)
    {
        ptr->setF(newF);
    }
}

double MagneticFieldElementsGetDecl(MagneticFieldElements *ptr) {
    if (ptr != nullptr)
    {
        return ptr->getDecl();
    }
    return -1;
}

void MagneticFieldElementsSetDecl(MagneticFieldElements *ptr, double newDecl) {
    if (ptr != nullptr)
    {
        ptr->setDecl(newDecl);
    }
}

double MagneticFieldElementsGetIncl(MagneticFieldElements *ptr) {
    if (ptr != nullptr)
    {
        return ptr->getIncl();
    }
    return -1;
}

void MagneticFieldElementsSetIncl(MagneticFieldElements *ptr, double newIncl) {
    if (ptr != nullptr)
    {
        ptr->setIncl(newIncl);
    }
}

void DisposeMagneticElements(MagneticFieldElements *ptr) {
    if (ptr != nullptr) {
        delete ptr;
        ptr = nullptr;
    }
}*/