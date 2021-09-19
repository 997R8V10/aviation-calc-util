%module mod_geopoint

%{
#include "GeoTools/GeoPoint.h"
%};

// Replace Unique Ptr
%ignore intersection;
%ignore getLat;
%ignore setLat;
%ignore getLon;
%ignore setLon;
%ignore getAlt;
%ignore setAlt;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Imports
%include <attribute.i>
%include <windows.i>
%include <typemaps.i>
%include "aviationcalc_exports.h"

// Main Import
%include "GeoTools/GeoPoint.h"

// C# Properties
%attribute(AviationCalcUtil::GeoTools::GeoPoint, double, Lat, getLat, setLat)
%attribute(AviationCalcUtil::GeoTools::GeoPoint, double, Lon, getLon, setLon)
%attribute(AviationCalcUtil::GeoTools::GeoPoint, double, Alt, getAlt, setAlt)

//Replace Unique Ptr
%extend AviationCalcUtil::GeoTools::GeoPoint {
    static GeoPoint *Intersection(const GeoPoint &point1, double bearing1, const GeoPoint &point2, double bearing2){
        return AviationCalcUtil::GeoTools::GeoPoint::intersection(point1, bearing1, point2, bearing2).release();
    }
}