%module mod_gribtile
%nspace AviationCalcUtil::GeoTools::GribTools::GribTile;

%{
#include "GeoTools/GeoTile.h"
#include "GeoTools/GeoPoint.h"
#include "GeoTools/GribTools/GribDataPoint.h"
#include "GeoTools/GribTools/GribTile.h"
%};

// Imports
%include <attribute.i>
%include <typemaps.i>
%include <swigtype_inout.i>
%include <std_string.i>
%include <std_shared_ptr.i>
%import ../../tools/boost_ptime.i
%import ../GeoPoint.i
%import ../GeoTile.i
%import GribDataPoint.i

// Shared Ptr
%shared_ptr(AviationCalcUtil::GeoTools::GribTools::GribTile);

// Remove default constructor
%nodefaultctor AviationCalcUtil::GeoTools::GribTools::GribTile;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Out variable handling
%apply double &OUTPUT { double & };
%apply SWIGTYPE *& OUTPUT { SWIGTYPE *& };

// Main Import
#define AVIATIONCALC_EXPORT
%include "GeoTools/GribTools/GribTile.h"

%clear double &;
%clear SWIGTYPE *&;