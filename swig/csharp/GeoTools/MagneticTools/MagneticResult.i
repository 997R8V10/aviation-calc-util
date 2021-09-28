%module mod_magneticresult

// Source header
%{
#include "GeoTools/MagneticTools/MagneticResult.h"
%};

// Imports
%include <attribute.i>
%import ../../tools/boost_gregorian_date.i
%import MagneticModel.i
%import MagneticFieldElements.i
%import ../GeoPoint.i

// Boost Date
%boost_gregorian_date();

// Rename to C# standard
%rename("%(camelcase)s", %$isfunction) "";

// C# Properties
%attributeref(AviationCalcUtil::GeoTools::MagneticTools::MagneticResult, %arg(const AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements &), MainFieldElements, getMainFieldElements);
%attributeref(AviationCalcUtil::GeoTools::MagneticTools::MagneticResult, %arg(const AviationCalcUtil::GeoTools::MagneticTools::MagneticFieldElements &), SecularFieldElements, getSecularFieldElements);

// Main Import for wrapper
#define AVIATIONCALC_EXPORT
%include "GeoTools/MagneticTools/MagneticResult.h"