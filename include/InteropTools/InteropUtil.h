//
// Created by pshivaraman on 2021-09-14.
//

#ifndef AVIATION_CALC_UTIL_INTEROPUTIL_H
#define AVIATION_CALC_UTIL_INTEROPUTIL_H

#include "aviationcalc_exports.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

struct InteropDateStruct {
    int year;
    int month;
    int day;
};

unsigned long long InteropBoostTimeToNs(boost::posix_time::ptime boost_time);

boost::posix_time::ptime InteropNsToBoostTime(unsigned long long ns);

InteropDateStruct InteropBoostDateToStruct(boost::gregorian::date boost_date);

boost::gregorian::date InteropStructToBoostDate(InteropDateStruct dStruct);

#endif //AVIATION_CALC_UTIL_INTEROPUTIL_H
