%module mod_magnetictile
%nspace AviationCalcUtil::GeoTools::MagneticTools::MagneticTile;

%{
#include "GeoTools/GeoTile.h"
#include "GeoTools/GeoPoint.h"
#include "GeoTools/MagneticTools/MagneticResult.h"
#include "GeoTools/MagneticTools/MagneticTile.h"
%};

// Imports
%include <attribute.i>
%include <typemaps.i>
%include <swigtype_inout.i>
%include <std_shared_ptr.i>
%import ../../tools/boost_gregorian_date.i
%import ../GeoPoint.i
%import ../GeoTile.i
%import MagneticResult.i

// Boost Date
%boost_gregorian_date();

// Shared Ptr
%shared_ptr(AviationCalcUtil::GeoTools::MagneticTools::MagneticTile);

// Remove default constructor
%nodefaultctor AviationCalcUtil::GeoTools::MagneticTools::MagneticTile;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Out variable handling
%apply double &OUTPUT { double & };
%apply SWIGTYPE *& OUTPUT { SWIGTYPE *& };
%apply std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & OUTPUT { std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & };

// Main Import
#define AVIATIONCALC_EXPORT
%include "GeoTools/MagneticTools/MagneticTile.h"

%clear double &;
%clear SWIGTYPE *&;
%clear std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> &;