//
// Created by pshivaraman on 2021-08-27.
//

#include <iostream>
#include <memory>
#include <GeoTools/GeoPoint.h>
#include <GeoTools/MagneticTools/MagneticUtil.h>

using namespace std;
using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::GeoTools::MagneticTools;

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

int main(){
    cout << "GeoPoint Tests" << endl;
    test_distanceM_1();
    test_intersection_1();
    test_intersection_2();

    cout << "Magnetic Tests" << endl;
    test_loadData();

    return 0;
}

