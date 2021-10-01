%module mod_magutil
%nspace AviationCalcUtil::GeoTools::MagneticTools::MagneticUtil;

%{
#include "GeoTools/MagneticTools/MagneticResult.h"
#include "GeoTools/GeoPoint.h"
#include "GeoTools/MagneticTools/MagneticUtil.h"
    %};

// Imports
%include <attribute.i>
%include <typemaps.i>
%include <swigtype_inout.i>
%import ../../tools/boost_gregorian_date.i
%import ../GeoPoint.i
%import MagneticResult.i

// Boost Date
%boost_gregorian_date();

// Remove default constructor
%nodefaultctor AviationCalcUtil::GeoTools::MagneticTools::MagneticUtil;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Out variable handling
%apply double &OUTPUT { double & };
%apply SWIGTYPE *& OUTPUT { SWIGTYPE *& };

// Main Import
#define AVIATIONCALC_EXPORT
%include "GeoTools/MagneticTools/MagneticUtil.h"

%clear double &;
%clear SWIGTYPE *&;