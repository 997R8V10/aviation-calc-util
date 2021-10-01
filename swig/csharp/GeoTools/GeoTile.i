%module mod_geotile
%nspace AviationCalcUtil::GeoTools::GeoTile;

%{
#include "GeoTools/GeoTile.h"
#include "GeoTools/GeoPoint.h"
#include <memory>
%};

// Imports
%include <attribute.i>
%include <typemaps.i>
%include <swigtype_inout.i>
%include <std_shared_ptr.i>
%import "GeoPoint.i"

// Remove default constructor
%nodefaultctor AviationCalcUtil::GeoTools::GeoTile;

// Shared Ptr
%shared_ptr(AviationCalcUtil::GeoTools::GeoTile);

// Ignores
%ignore deprecated;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Out variable handling
%apply double &OUTPUT { double & };
%apply SWIGTYPE *& OUTPUT { SWIGTYPE *& };
%apply std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & OUTPUT { std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> & };

// C# Properties
%attribute_unique_ptr(AviationCalcUtil::GeoTools::GeoTile, AviationCalcUtil::GeoTools::GeoPoint, CenterPoint, getCenterPoint)
%attribute(AviationCalcUtil::GeoTools::GeoTile, double, TopLat, getTopLat)
%attribute(AviationCalcUtil::GeoTools::GeoTile, double, BottomLat, getBottomLat)
%attribute(AviationCalcUtil::GeoTools::GeoTile, double, LeftLon, getLeftLon)
%attribute(AviationCalcUtil::GeoTools::GeoTile, double, RightLon, getRightLon)

// Main Import
#define AVIATIONCALC_EXPORT
%include "GeoTools/GeoTile.h"

%clear double &;
%clear SWIGTYPE *&;
%clear std::unique_ptr<AviationCalcUtil::GeoTools::GeoPoint> &;