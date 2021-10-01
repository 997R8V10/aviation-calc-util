%module mod_magneticmodelcoeffs
%nspace AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients;

// Source header
%{
#include "GeoTools/MagneticTools/MagneticModelCoefficients.h"
%};

// Imports
%include <attribute.i>
%include ../../tools/std_unique_ptr.i
%include <std_shared_ptr.i>
%import ../../tools/boost_gregorian_date.i

// Boost Date
%boost_gregorian_date();

// Shared Pointer
%shared_ptr(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients);

// Unique Ptr
%unique_ptr(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients);

// Rename to C# standard
%rename("%(camelcase)s", %$isfunction) "";

// C# Properties
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients, double, Degree, getN);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients, double, Order, getM);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients, double, MainFieldG, getG);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients, double, MainFieldH, getH);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients, double, SecularVariationG, getGDot);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients, double, SecularVariationH, getHDot);

// Main Import for wrapper
#define AVIATIONCALC_EXPORT
%include "GeoTools/MagneticTools/MagneticModelCoefficients.h"