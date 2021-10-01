%module mod_magneticfieldelements
%nspace AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements;

// Source header
%{
#include "GeoTools/MagneticTools/MagneticFieldElements.h"
%};

// Imports
%include <attribute.i>
%include ../../tools/std_unique_ptr.i

// Unique Ptr
%unique_ptr(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements);

// Rename to C# standard
%rename("%(camelcase)s", %$isfunction) "";

// C# Properties
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements, double, NorthComponent, getX, setX);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements, double, EastComponent, getY, setY);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements, double, DownComponent, getZ, setZ);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements, double, HorizontalIntensity, getH, setH);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements, double, TotalIntensity, getF, setF);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements, double, Declination, getDecl, setDecl);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements, double, Inclination, getIncl, setIncl);

// Main Import for wrapper
#define AVIATIONCALC_EXPORT
%include "GeoTools/MagneticTools/MagneticFieldElements.h"