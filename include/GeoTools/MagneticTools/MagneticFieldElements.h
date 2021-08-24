//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H
#define AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H

namespace AviationCalcUtil::GeoTools::MagneticTools{
class MagneticFieldElements{
private:
    double x;
    double y;
    double z;
    double h;
    double f;
    double decl;
    double incl;

public:
    MagneticFieldElements();

    double getX();
    void setX(double newX);
    double getY();
    void setY(double newY);
    double getZ();
    void setZ(double newZ);
    double getH();
    void setH(double newH);
    double getF();
    void setF(double newF);
    double getDecl();
    void setDecl(double newDecl);
    double getIncl();
    void setIncl(double newIncl);
};
}

#endif //AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H
