%module mod_interop_boost_date

%define %boost_gregorian_date()
    // Include boost headers
    %{
#include <boost/date_time/gregorian/gregorian.hpp>

        namespace AviationCalcUtil::InteropTools {
            struct InteropDateStruct {
                int year;
                int month;
                int day;
            };
        }
    %}

    %typemap (ctype) const boost::gregorian::date & "AviationCalcUtil::InteropTools::InteropDateStruct"
    %typemap (imtype, out="global::AviationCalcUtilNet.InteropTools.InteropDateStruct") const boost::gregorian::date & "global::AviationCalcUtilNet.InteropTools.InteropDateStruct"
    %typemap (cstype) const boost::gregorian::date & "global::System.DateTime"

    %typemap(csdirectorin) const boost::gregorian::date & "$iminput"
    %typemap (in) const boost::gregorian::date & %{
        boost::gregorian::date temp = boost::gregorian::date($input.year, $input.month, $input.day);
        $1 = &temp;
    %}
    %typemap (out) const boost::gregorian::date & %{
        AviationCalcUtil::InteropTools::InteropDateStruct s{};
        s.year = $1.year();
        s.month = $1.month();
        s.day = $1.day();

        $result = s;
    %}
    %typemap(csin,
             pre="    global::System.DateTime temp$csinput = $csinput.ToUniversalTime(); var tempStr = new global::AviationCalcUtilNet.InteropTools.InteropDateStruct(){ year = temp$csinput.Year, month = temp$csinput.Month, day = temp$csinput.Day };"
             ) const boost::gregorian::date & "tempStr"
    %typemap(csout, excode=SWIGEXCODE) const boost::gregorian::date & {
        InteropDateStruct s = $imcall;
        global::System.DateTime ret = new global::System.DateTime(s);$excode
        return global::System.DateTime.SpecifyKind(ret, global::System.DateTimeKind.Utc);
    }
%enddef

// InteropDateStruct
%nodefaultctor AviationCalcUtil::InteropTools::InteropDateStruct; // Remove default constructor
%nodefaultdtor AviationCalcUtil::InteropTools::InteropDateStruct; // Remove default destructor
%nonspace AviationCalcUtil::InteropTools::InteropDateStruct;
%typemap(csclassmodifiers) AviationCalcUtil::InteropTools::InteropDateStruct "internal struct" // Change from default public class
%typemap(csinterfaces) AviationCalcUtil::InteropTools::InteropDateStruct "" // Override default IDisposable inheritance
%typemap(csbody) AviationCalcUtil::InteropTools::InteropDateStruct %{
    public int year;
    public int month;
    public int day;
%}
%typemap(csdispose) AviationCalcUtil::InteropTools::InteropDateStruct "" // Remove default Dispose method
%typemap(csdisposing) AviationCalcUtil::InteropTools::InteropDateStruct "" // Remove default Dispose method

namespace AviationCalcUtil::InteropTools{
    struct InteropDateStruct {
    };
}