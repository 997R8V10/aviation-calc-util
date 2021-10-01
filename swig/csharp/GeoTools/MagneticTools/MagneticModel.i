%module mod_magneticmodel
%nspace AviationCalcUtil::GeoTools::MagneticTools::MagneticModel;

// Source header
%{
#include "GeoTools/MagneticTools/MagneticModel.h"
%};

// Imports
%include <attribute.i>
%include <std_shared_ptr.i>
%include <std_string.i>
%import ../../tools/boost_gregorian_date.i
%import MagneticModelCoefficients.i

// Vectors
%include ../../tools/std_vector_extend.i
%std_vector_extend(VectorCoeffs, std::shared_ptr<AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients>);
%std_vector_apply(VectorCoeffs, std::shared_ptr<AviationCalcUtil::GeoTools::MagneticTools::MagneticModelCoefficients>, coefficients);

// Boost Date
%boost_gregorian_date();

// Shared Ptr
%shared_ptr(AviationCalcUtil::GeoTools::MagneticTools::MagneticModel);

// Rename to C# standard
%rename("%(camelcase)s", %$isfunction) "";

// C# Properties
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModel, double, ModelEpoch, getModelEpoch);
%attributestring(AviationCalcUtil::GeoTools::MagneticTools::MagneticModel, %arg(std::string), ModelName, getModelName);
%attribute(AviationCalcUtil::GeoTools::MagneticTools::MagneticModel, %arg(boost::gregorian::date), ReleaseDate, getReleaseDate);

// Main Import for wrapper
#define AVIATIONCALC_EXPORT
%include "GeoTools/MagneticTools/MagneticModel.h"