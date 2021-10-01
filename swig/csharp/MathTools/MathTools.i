%module mod_mathtools

// Namespaces
%nspace AviationCalcUtil::MathTools::LegendreUtil;
%nspace AviationCalcUtil::MathTools::Polynomial;
%nspace AviationCalcUtil::MathTools::MathUtil;

// C++ Includes
%{
#include "MathTools/Polynomial.h"
#include "MathTools/LegendreUtil.h"
#include "MathTools/MathUtil.h"
%};

// Imports
%include <attribute.i>;
%include <std_shared_ptr.i>;

// Shared Pointers
%shared_ptr(AviationCalcUtil::MathTools::Polynomial);

// Vectors
%include "../tools/std_vector_extend.i";
%std_vector_extend(DoubleVectorCoeffs, double);
%std_vector_apply(DoubleVectorCoeffs, double, coefficients);
%std_vector_apply(DoubleVectorCoeffs, double, Coefficients);

// Rename to C# standard
%rename("%(camelcase)s", %$isfunction) "";

// C# Properties
%attributeref(AviationCalcUtil::MathTools::Polynomial, %arg(const std::vector<double> &), Coefficients, getCoefficients);

// Main Import for wrapper
#define AVIATIONCALC_EXPORT
%include "MathTools/Polynomial.h";
%include "MathTools/LegendreUtil.h";
%include "MathTools/MathUtil.h";