%include <typemaps.i>
%include <attribute.i>
%include <std_vector.i>

%define %std_vector_extend(NAME, TYPE)
    SWIG_STD_VECTOR_ENHANCED(TYPE);
    %typemap (cstype) const std::vector<TYPE> & "global::System.Collections.IList<$typemap(cstype, TYPE)>"

    %typemap(csin,
             pre="    NAME temp$csinput = new NAME($csinput);"
                     ) const std::vector<TYPE> & "temp$csinput"
    namespace std {
        %template(NAME) vector<TYPE>;
    };
%enddef