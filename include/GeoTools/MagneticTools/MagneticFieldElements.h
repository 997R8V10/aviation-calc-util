//
// Created by Prithvisagar Shivaraman on 8/24/21.
//

#ifndef AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H
#define AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H

#include "aviationcalc_exports.h"

namespace AviationCalcUtil::GeoTools::MagneticTools {
    class AVIATIONCALC_EXPORT MagneticFieldElements {
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

        MagneticFieldElements(const MagneticFieldElements& other);

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
/*
using namespace AviationCalcUtil::GeoTools::MagneticTools;

extern "C"
{
extern AVIATIONCALC_EXPORT MagneticFieldElements *CreateMagneticFieldElements();
extern AVIATIONCALC_EXPORT double MagneticFieldElementsGetX(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT void MagneticFieldElementsSetX(MagneticFieldElements* ptr, double newX);
extern AVIATIONCALC_EXPORT void DisposeMagneticElements(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT double MagneticFieldElementsGetY(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT void MagneticFieldElementsSetY(MagneticFieldElements* ptr, double newY);
extern AVIATIONCALC_EXPORT double MagneticFieldElementsGetZ(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT void MagneticFieldElementsSetZ(MagneticFieldElements* ptr, double newZ);
extern AVIATIONCALC_EXPORT double MagneticFieldElementsGetH(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT void MagneticFieldElementsSetH(MagneticFieldElements* ptr, double newH);
extern AVIATIONCALC_EXPORT double MagneticFieldElementsGetF(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT void MagneticFieldElementsSetF(MagneticFieldElements* ptr, double newF);
extern AVIATIONCALC_EXPORT double MagneticFieldElementsGetDecl(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT void MagneticFieldElementsSetDecl(MagneticFieldElements* ptr, double newDecl);
extern AVIATIONCALC_EXPORT double MagneticFieldElementsGetIncl(MagneticFieldElements* ptr);
extern AVIATIONCALC_EXPORT void MagneticFieldElementsSetIncl(MagneticFieldElements* ptr, double newIncl);
}*/

#endif //AVIATION_CALC_UTIL_MAGNETICFIELDELEMENTS_H
