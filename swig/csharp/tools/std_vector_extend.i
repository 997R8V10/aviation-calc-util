%include <typemaps.i>
%include <attribute.i>
%include <std_vector.i>

%define %std_vector_extend(NAME, TYPE)
    %rename("%s", %$isfunction) "";
    namespace std {
        %template(NAME) vector<TYPE>;
    };
%enddef

%define %std_vector_apply(NAME, TYPE, ARG)
    %rename("%s", %$isfunction) "";
    %typemap (cstype) const std::vector<TYPE> & ARG "global::System.Collections.Generic.IEnumerable<$typemap(cstype, TYPE)>"

    %typemap(csin,
         pre="    NAME temp$csinput = new NAME($csinput);"
                 ) const std::vector<TYPE> & ARG "$csclassname.getCPtr(temp$csinput)"
%enddef