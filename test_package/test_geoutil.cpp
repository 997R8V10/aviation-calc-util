//
// Created by Prithvi Shivaraman on 10/15/23.
//

#include <iostream>
#include <GeoTools/GeoUtil.h>
#include <GeoTools/GeoPoint.h>

using namespace std;
using namespace AviationCalcUtil::GeoTools;

void test_calculateDeltaToHeading_1(){
    cout << "Test calculateDeltaToHeading 001:" << endl;
    cout << "\t350\t=>\t010\tR" << endl;
    cout << "\tExpected:\t20" << endl;
    double deltaToHeading = GeoUtil::calculateDeltaToHeading(350, 10, true);
    cout << "\tCalculated:\t" << deltaToHeading << "degs" << endl;
    cout << endl;
}

void test_calculateDeltaToHeading_2(){
    cout << "Test calculateDeltaToHeading 002:" << endl;
    cout << "\t010\t=>\t350\tL" << endl;
    cout << "\tExpected:\t20" << endl;
    double deltaToHeading = GeoUtil::calculateDeltaToHeading(10, 350, false);
    cout << "\tCalculated:\t" << deltaToHeading << "degs" << endl;
    cout << endl;
}

void test_calculateDeltaToHeading_3(){
    cout << "Test calculateDeltaToHeading 003:" << endl;
    cout << "\t010\t=>\t350\tR" << endl;
    cout << "\tExpected:\t340" << endl;
    double deltaToHeading = GeoUtil::calculateDeltaToHeading(10, 350, true);
    cout << "\tCalculated:\t" << deltaToHeading << "degs" << endl;
    cout << endl;
}

void test_calculateDeltaToHeading_4(){
    cout << "Test calculateDeltaToHeading 004:" << endl;
    cout << "\t350\t=>\t010\tL" << endl;
    cout << "\tExpected:\t340" << endl;
    double deltaToHeading = GeoUtil::calculateDeltaToHeading(350, 10, false);
    cout << "\tCalculated:\t" << deltaToHeading << "degs" << endl;
    cout << endl;
}

void test_calculateArcCourseInfo_1(){
    cout << "Test calculateArcCourseInfo 001:" << endl;
    cout << "\tCenter: (38, -77)\tRadius: 10km\tR-250 => R-030\tCur: R-280 (11km radius)" << endl;
    cout << "\tExpected:\tReq'd Course: 010\tXTK: -1000m\tATK: 19198.62m" << endl;
    GeoPoint ctr(38, -77, 0);
    GeoPoint acft(38, -77, 0);
    acft.moveByM(280, 11000);
    double reqdCourse = -1;
    double aTk_m = -2;
    double xTk_m = GeoUtil::calculateArcCourseInfo(acft, ctr, 250, 30, 10000, true, reqdCourse, aTk_m);
    cout << "\tCalculated:\tReq'd Course: " << reqdCourse << "\tXTK: " << xTk_m << "m\tATK: " << aTk_m << "m" << endl;
    cout << endl;
}

int main(){
    cout << "GeoUtil Tests" << endl;
    test_calculateDeltaToHeading_1();
    test_calculateDeltaToHeading_2();
    test_calculateDeltaToHeading_3();
    test_calculateDeltaToHeading_4();
    test_calculateArcCourseInfo_1();

    return 0;
}
