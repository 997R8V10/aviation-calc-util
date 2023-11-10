//
// Created by pshivaraman on 2021-08-27.
//

#include <iostream>
#include <memory>
#include <GeoTools/GeoPoint.h>
#include <GeoTools/MagneticTools/MagneticUtil.h>
#include <GeoTools/GribTools/GribTile.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::GeoTools::MagneticTools;
using namespace boost::posix_time;
using namespace boost::gregorian;

void test_distanceM_1(){
    cout << "Test Distance(M) 001:" << endl;
    cout << "\t{0, 0, 0}\t=>\t{1, 0, 0}" << endl;
    cout << "\tExpected:\t111120m" << endl;
    GeoPoint pt1(0, 0, 0);
    GeoPoint pt2(1, 0, 0);
    double distanceM = GeoPoint::distanceM(pt1, pt2);
    cout << "\tCalculated:\t" << distanceM << "m" << endl;
    cout << endl;
}

void test_distanceM_2() {
    cout << "Test Distance(M) 002:" << endl;
    cout << "\t{-54.732690528309909, 173.66679109972142, 0}\t=>\t{54.732690543659544, -6.33320892249435, 0}" << endl;
    cout << "\tExpected:\tA large number" << endl;
    GeoPoint pt1(-54.732690528309909, 173.66679109972142, 0);
    GeoPoint pt2(54.732690543659544, -6.33320892249435, 0);
    double distanceM = GeoPoint::flatDistanceM(pt1, pt2);
    cout << "\tCalculated:\t" << distanceM << "m" << endl;
    cout << endl;
}

void test_distanceM_3() {
    cout << "Test Distance(M) 003:" << endl;
    cout << "\t{-45, 170, 0}\t=>\t{45, -10, 0}" << endl;
    cout << "\tExpected:\tA large number" << endl;
    GeoPoint pt1(-45, 170, 0);
    GeoPoint pt2(45, -10, 0);
    double distanceM = GeoPoint::distanceM(pt1, pt2);
    cout << "\tCalculated:\t" << distanceM << "m" << endl;
    cout << endl;
}

void test_intersection_1(){
    cout << "Test Intersection 001:" << endl;
    cout << "\t{0, 0, 0} 045\t=>\t{1, 0, 0} 315" << endl;
    GeoPoint pt1(0, 0, 0);
    GeoPoint pt2(1, 0, 0);
    unique_ptr<GeoPoint> interPt = GeoPoint::intersection(pt1, 45, pt2, 315);
    cout << "\tExpected:\tNo Intersection Found!" << endl;
    if (interPt == nullptr){
        cout << "\tCalculated:\tNo Intersection Found!" << endl;
    } else {
        cout << "\tCalculated:\t{" << interPt->getLat() << ", " << interPt->getLon() << ", " << interPt->getAlt() << "}" << endl;
    }
    cout << endl;
}

void test_intersection_2(){
    cout << "Test Intersection 002:" << endl;
    cout << "\t{0, 0, 0} 045\t=>\t{0, 1, 0} 315" << endl;
    GeoPoint pt1(0, 0, 0);
    GeoPoint pt2(0, 1, 0);
    unique_ptr<GeoPoint> interPt = GeoPoint::intersection(pt1, 45, pt2, 315);
    cout << "\tExpected:\t{0.5, 0.5, 0}" << endl;
    if (interPt == nullptr){
        cout << "\tCalculated:\tNo Intersection Found!" << endl;
    } else {
        cout << "\tCalculated:\t{" << interPt->getLat() << ", " << interPt->getLon() << ", " << interPt->getAlt() << "}" << endl;
    }
    cout << endl;
}

void test_loadData(){
    cout << "Test Load Data:" << endl;
    try {
        MagneticUtil::loadData();
        cout << "\t Loaded Succesfully!" << endl;
    } catch (const std::invalid_argument &ex){
        cout << "\t" << ex.what() << endl;
    }
    cout <<  endl;
}

void test_fetchGrib(){
    cout << "Test Grib Data:" << endl;
    cout << "\t{54, -6, 30000} Grib Tile" << endl;
    GeoPoint pt1(54, -6, 30000);
    ptime dt(date(2021, 9, 17), time_duration(0, 0, 0));
    auto gribTile = GribTools::GribTile::findOrCreateGribTile(pt1, dt);

    if (gribTile == nullptr){
        cout << "\tFailed to get grib tile!";
    } else {
        cout << "\tFilename:\t" << gribTile->getGribFileName();
    }
    cout <<  endl;
}

int main(){
    cout << "GeoPoint Tests" << endl;
    test_distanceM_1();
    test_distanceM_2();
    test_distanceM_3();
    test_intersection_1();
    test_intersection_2();

    cout << "Magnetic Tests" << endl;
    test_loadData();

    cout << "Grib Tests" << endl;
    test_fetchGrib();

    return 0;
}

