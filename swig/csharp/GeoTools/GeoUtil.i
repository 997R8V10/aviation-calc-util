%module mod_geoutil
%nspace AviationCalcUtil::GeoTools::GeoUtil;

%{
#include "GeoTools/GeoUtil.h"
#include "GeoTools/GeoPoint.h"
#include <memory>
%};

// Imports
%include <attribute.i>
%include <typemaps.i>
%include <swigtype_inout.i>
%include ../tools/csharp_static_class.i
%import GeoPoint.i

// Make static class
%csharp_static_class(AviationCalcUtil::GeoTools::GeoUtil);

// Ignores
%ignore deprecated;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Out variable handling
%apply double &OUTPUT { double & };
%apply SWIGTYPE *& OUTPUT { SWIGTYPE *& };
%apply std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & OUTPUT { std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & };

// Main Import
#define AVIATIONCALC_EXPORT
%include "GeoTools/GeoUtil.h"

%clear double &;
%clear SWIGTYPE *&;
%clear std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> &;