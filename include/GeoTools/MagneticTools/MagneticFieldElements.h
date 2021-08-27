//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H
#define AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class MagneticFieldElements {
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

        double getX() const;

        void setX(double newX);

        double getY() const;

        void setY(double newY);

        double getZ() const;

        void setZ(double newZ);

        double getH() const;

        void setH(double newH);

        double getF() const;

        void setF(double newF);

        double getDecl() const;

        void setDecl(double newDecl);

        double getIncl() const;

        void setIncl(double newIncl);
    };
}

#endif //AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H
