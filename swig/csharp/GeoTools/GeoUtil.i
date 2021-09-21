%module mod_geoutil

%{
#include "GeoTools/GeoUtil.h"
#include "GeoTools/GeoPoint.h"
#include <memory>
%};

// Imports
%include <attribute.i>
%include <typemaps.i>
%include <swigtype_inout.i>
%import GeoPoint.i

// Remove default constructor
%nodefaultctor GeoUtil;
%nspace AviationCalcUtil::GeoTools;

// Ignores
%ignore deprecated;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Out variable handling
%apply double &OUTPUT { double & };
%apply SWIGTYPE *& OUTPUT { SWIGTYPE *& };
%apply std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & OUTPUT { std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & };

// Main Import
%include "GeoTools/GeoUtil.h"

//Replace Unique Ptr
%extend AviationCalcUtil::GeoTools::GeoUtil {
}

%clear double &;
%clear std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> &;