//
// Created by pshivaraman on 2021-09-14.
//

#include <InteropTools/InteropUtil.h>
using namespace boost::gregorian;
using namespace boost::posix_time;

unsigned long long InteropBoostTimeToNs(boost::posix_time::ptime boost_time) {
    //C# offset till 1400.01.01 00:00:00
    uint64_t netEpochOffset = 441481536000000000LL;

    // Boost 0 boost_time
    ptime ptimeEpoch(date(1400, 1, 1), time_duration(0, 0, 0));

    // Get Nanoseconds since boost epoch
    time_duration td = boost_time - ptimeEpoch;
    uint64_t nano = td.total_microseconds() * 10LL;
    return nano + netEpochOffset;
}

boost::posix_time::ptime InteropNsToBoostTime(unsigned long long ns) {
    // Boost 0 time
    ptime ptimeEpoch(date(1400, 1, 1), time_duration(0, 0, 0));

    return ptimeEpoch + microseconds(ns / 10LL);
}

InteropDateStruct InteropBoostDateToStruct(boost::gregorian::date boost_date) {
    InteropDateStruct s{};
    s.year = boost_date.year();
    s.month = boost_date.month();
    s.day = boost_date.day();

    return s;
}

boost::gregorian::date InteropStructToBoostDate(InteropDateStruct dStruct) {
    return date(dStruct.year, dStruct.month, dStruct.day);
}

