%module mod_atmosutil
%nspace AviationCalcUtil::GeoTools::AtmosUtil;

%{
#include "GeoTools/AtmosUtil.h"
%};

// Imports
%include <typemaps.i>

// Remove default constructor
%nodefaultctor AtmosUtil;
%nspace AviationCalcUtil::GeoTools;

// Ignores
%ignore deprecated;

// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Out variable handling
%apply double &OUTPUT { double & };

// Main Import
#define AVIATIONCALC_EXPORT
%include "GeoTools/AtmosUtil.h"

%clear double &;