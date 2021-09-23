%define %csharp_static_class(TYPE)
    %nodefaultctor TYPE; // Remove default constructor
%enddef