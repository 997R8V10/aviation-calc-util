%module mod_geopoint

// Source header
%{
#include "GeoTools/GeoPoint.h"
%};

// Imports
%include <attribute.i>
%include <typemaps.i>
%include ../tools/std_unique_ptr.i

// Unique Ptr
%unique_ptr(AviationCalcUtil::GeoTools::GeoPoint);

// Rename to C# standard
%rename("%(camelcase)s", %$isfunction) "";

// C# Properties
%attribute(AviationCalcUtil::GeoTools::GeoPoint, double, Lat, getLat, setLat)
%attribute(AviationCalcUtil::GeoTools::GeoPoint, double, Lon, getLon, setLon)
%attribute(AviationCalcUtil::GeoTools::GeoPoint, double, Alt, getAlt, setAlt)

// Main Import for wrapper
#define AVIATIONCALC_EXPORT
%include "GeoTools/GeoPoint.h"