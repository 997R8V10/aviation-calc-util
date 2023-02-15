/* -----------------------------------------------------------------------------
 * std_string.i
 *
 * Typemaps for std::string and const std::string&
 * These are mapped to a C# String and are passed around by value.
 *
 * To use non-const std::string references use the following %apply.  Note 
 * that they are passed by value.
 * %apply const std::string & {std::string &};
 * ----------------------------------------------------------------------------- */

%{
#include <string>
%}

namespace std {

%naturalvar string;

class string;

// Out var handling
%typemap (ctype) std::string &OUTPUT "char **"
%typemap (imtype) std::string &OUTPUT "out string"
%typemap (cstype) std::string &OUTPUT "out string"

%typemap(csin) std::string &OUTPUT "out $csinput"

%typemap(in, canthrow=1) std::string &OUTPUT (std::string temp) %{ $1 = &temp; %}
%typemap(argout) std::string &OUTPUT %{ *$input = SWIG_csharp_string_callback($1->c_str()); %}


%typemap(throws, canthrow=1) string &OUTPUT
%{ SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.c_str());
   return $null; %}

}