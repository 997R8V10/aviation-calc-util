%include <typemaps.i>
%include <attribute.i>

// Include boost headers
%{
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
%}

/**
 * const boost::posix_time::ptime &
 */
// Type definitions
%typemap (ctype) const boost::posix_time::ptime & "uint64_t"
%typemap (imtype, out="ulong") const boost::posix_time::ptime & "ulong"
%typemap (cstype) const boost::posix_time::ptime & "global::System.DateTime"

// C In Conversion
%typemap(csdirectorin) const boost::posix_time::ptime & "$iminput"
%typemap (in) const boost::posix_time::ptime & %{
    // Boost 0 time
    boost::posix_time::ptime ptimeEpoch(boost::gregorian::date(1400, 1, 1), boost::posix_time::time_duration(0, 0, 0));

    return ptimeEpoch + boost::posix_time::microseconds($input / 10LL);
%}

// C Out conversion
%typemap (out) const boost::posix_time::ptime & %{
    //C# offset till 1400.01.01 00:00:00
    uint64_t netEpochOffset = 441481536000000000LL;

    // Boost 0 boost_time
    boost::posix_time::ptime ptimeEpoch(boost::gregorian::date(1400, 1, 1), boost::posix_time::time_duration(0, 0, 0));

    // Get Nanoseconds since boost epoch
    boost::posix_time::time_duration td = $1 - ptimeEpoch;
    uint64_t nano = td.total_microseconds() * 10LL;
    $result = nano + netEpochOffset;
%}

// C# In Conversion
%typemap(csin, pre="global::System.DateTime temp$csinput = $csinput.ToUniversalTime();\n"
                   "\n"
                   "    //C# offset till 1400.01.01 00:00:00\n"
                   "    long netEpochOffset = 441481536000000000L;\n"
                   "\n"
                   "    // Get Nanoseconds since boost epoch\n"
                   "    ulong result$csinput = (ulong) (temp$csinput.Ticks - netEpochOffset);"
                   ) const boost::posix_time::ptime & "result$csinput";

// C# Out Conversion
%typemap(csout, excode=SWIGEXCODE) const boost::posix_time::ptime & {
    ulong ns = $imcall;
    global::System.DateTime ret = new global::System.DateTime((long)ns, DateTimeKind.Utc);$excode
    return ret;
}

/**
 * boost::posix_time::ptime
 */
// Type definitions
%typemap (ctype) boost::posix_time::ptime "uint64_t"
%typemap (imtype, out="ulong") boost::posix_time::ptime "ulong"
%typemap (cstype) boost::posix_time::ptime "global::System.DateTime"

// C In Conversion
%typemap(csdirectorin) boost::posix_time::ptime "$iminput"
%typemap (in) boost::posix_time::ptime %{
    // Boost 0 time
    boost::posix_time::ptime ptimeEpoch(boost::gregorian::date(1400, 1, 1), boost::posix_time::time_duration(0, 0, 0));

    return ptimeEpoch + boost::posix_time::microseconds($input / 10LL);
%}

// C Out conversion
%typemap (out) boost::posix_time::ptime %{
    //C# offset till 1400.01.01 00:00:00
    uint64_t netEpochOffset = 441481536000000000LL;

    // Boost 0 boost_time
    boost::posix_time::ptime ptimeEpoch(boost::gregorian::date(1400, 1, 1), boost::posix_time::time_duration(0, 0, 0));

    // Get Nanoseconds since boost epoch
    boost::posix_time::time_duration td = $1 - ptimeEpoch;
    uint64_t nano = td.total_microseconds() * 10LL;
    $result = nano + netEpochOffset;
%}

// C# In Conversion
%typemap(csin, pre="global::System.DateTime temp$csinput = $csinput.ToUniversalTime();\n"
                   "\n"
                   "    //C# offset till 1400.01.01 00:00:00\n"
                   "    long netEpochOffset = 441481536000000000L;\n"
                   "\n"
                   "    // Get Nanoseconds since boost epoch\n"
                   "    ulong result$csinput = (ulong) (temp$csinput.Ticks - netEpochOffset);"
                   ) boost::posix_time::ptime "result$csinput";

// C# Out Conversion
%typemap(csout, excode=SWIGEXCODE) boost::posix_time::ptime {
    ulong ns = $imcall;
    global::System.DateTime ret = new global::System.DateTime((long)ns, DateTimeKind.Utc);$excode
    return ret;
}

// Properties
%typemap(csvarin, excode=SWIGEXCODE) boost::posix_time::ptime %{
    /* csvarin typemap code */
    set {
        global::System.DateTime tempDt$csinput = $csinput.ToUniversalTime();

        //C# offset till 1400.01.01 00:00:00
        long netEpochOffset = 441481536000000000L;

        // Get Nanoseconds since boost epoch
        ulong temp$csinput = (ulong) (tempDt$csinput.Ticks - netEpochOffset);

        $imcall;$excode
    }
%}

%typemap(csvarout, excode=SWIGEXCODE) boost::posix_time::ptime %{
    /* csvarout typemap code */
    get {
        ulong ns = $imcall;
        global::System.DateTime ret = new global::System.DateTime((long)ns, DateTimeKind.Utc);$excode
        return ret;
    }
%}

namespace boost{
    namespace gregorian{

    };
    namespace posix_time{

    };
};