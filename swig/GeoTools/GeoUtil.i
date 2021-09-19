%module mod_geoutil

%{
#include "GeoTools/GeoUtil.h"
#include "GeoTools/GeoPoint.h"
#include <memory>
%};

%nodefaultctor GeoUtil;
%nspace AviationCalcUtil::GeoTools;

// Replace Unique Ptr
%ignore deprecated;
%ignore findIntersection;
%ignore calculateTurnLeadDistance;
// Renames
%rename("%(camelcase)s", %$isfunction) "";

// Imports
%include <attribute.i>
%include <windows.i>
%include <typemaps.i>
%include <swigtype_inout.i>
%import GeoPoint.i
%include "aviationcalc_exports.h"

// Out variable handling
%apply double &OUTPUT { double & };
%apply SWIGTYPE *& OUTPUT { SWIGTYPE *& };

// Main Import
%include "GeoTools/GeoUtil.h"

//Replace Unique Ptr
%extend AviationCalcUtil::GeoTools::GeoUtil {

    static double CalculateTurnLeadDistance(const GeoPoint &pos, const GeoPoint &wp, double trueTrack, double tas,
                                            double course, double trueWindDir, double windSpd, double &radiusOfTurn,
                                            GeoPoint *&intersection){
        auto nat_int = std::make_unique<AviationCalcUtil::GeoTools::GeoPoint>();
        double toReturn = AviationCalcUtil::GeoTools::GeoUtil::calculateTurnLeadDistance(pos, wp, trueTrack, tas, course, trueWindDir, windSpd, radiusOfTurn, nat_int);
        intersection = nat_int.release();
        return toReturn;
    }


    static GeoPoint *FindIntersection(const GeoPoint &position, const GeoPoint &wp,
                                                      double trueTrack, double course){
        return AviationCalcUtil::GeoTools::GeoUtil::findIntersection(position, wp, trueTrack, course).release();
    }
}
%clear double &;
%clear SWIGTYPE *&;