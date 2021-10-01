%module mod_gribdatapoint
%nspace AviationCalcUtil::GeoTools::GribTools::GribDataPoint;

// Source header
%{
#include "GeoTools/GeoPoint.h"
#include "GeoTools/GribTools/GribDataPoint.h"
%};

// Imports
%include <attribute.i>;
%include <std_shared_ptr.i>;
%include <std_string.i>
%include "../../tools/std_unique_ptr.i";
%import "../GeoPoint.i";

// Smart Pointers
%unique_ptr(AviationCalcUtil::GeoTools::GribTools::GribDataPoint);
%shared_ptr(AviationCalcUtil::GeoTools::GribTools::GribDataPoint);

// Rename to C# standard
%rename("%(camelcase)s", %$isfunction) "";

// Override ToString()
%csmethodmodifiers AviationCalcUtil::GeoTools::GribTools::GribDataPoint::toString "public override";

// C# Properties
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, Latitude, getLatitude);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, Longitude, getLongitude);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, Longitude_Norm, getLongitudeNormalized);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, GeoPotentialHeight_M, getGeoPotentialHeightM, setGeoPotentialHeightM);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, GeoPotentialHeight_Ft, getGeoPotentialHeightFt);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, Level_hPa, getLevelHPa);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, Temp_K, getTempK, setTempK);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, Temp_C, getTempC);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, V_mpers, getVMpers, setVMpers);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, U_mpers, getUMpers, setUMpers);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, WSpeed_mpers, getWindSpeedMpers);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, WSpeed_kts, getWindSpeedKts);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, WDir_rads, getWindDirRads);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, WDir_deg, getWindDirDegs);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, RelativeHumidity, getRelHumidity, setRelHumidity);
%attribute(AviationCalcUtil::GeoTools::GribTools::GribDataPoint, double, SfcPress_hPa, getSfcPressHPa, setSfcPressHPa);

// Main Import for wrapper
#define AVIATIONCALC_EXPORT
%include "GeoTools/GribTools/GribDataPoint.h";