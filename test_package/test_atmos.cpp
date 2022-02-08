//
// Created by pshivaraman on 2021-09-03.
//

#include <GeoTools/AtmosUtil.h>
#include <MathTools/MathUtil.h>
#include <GeoTools/GeoUtil.h>
#include <GeoTools/GribTools/GribTile.h>
#include <iostream>
#include <string>

using namespace AviationCalcUtil::GeoTools;
using namespace AviationCalcUtil::MathTools;
using namespace AviationCalcUtil::GeoTools::GribTools;
using namespace std;

void testTrueAirspeed1(double ias, double altFt, double satC){
    cout << "Test TAS 1:" << endl;
    cout << "\tIAS: " << ias << "kts;\tAlt: " << altFt << "ft" << endl;
    double onlineTas = 404.5;

    double h = MathUtil::convertFeetToMeters(altFt);
    //double T = AtmosUtil::calculateTempAtAlt(h, 0, AtmosUtil::ISA_STD_TEMP_K);
    double T = MathUtil::convertCelsiusToKelvin(satC);
    double p = AtmosUtil::calculatePressureAtAlt(h, 0, AtmosUtil::ISA_STD_PRES_Pa, T);
    double cas = MathUtil::convertKtsToMpers(ias);
    double qc = AtmosUtil::calculateImpactPressure(cas);
    double M = AtmosUtil::calculateMachNumber(qc, p);
    double eas = AtmosUtil::calculateEas(M, p);
    double atmosTas = AtmosUtil::convertMachToTas(M, T);

    cout << "\tMach: \t\t" << M << endl;
    cout << "\tEAS: \t\t" << MathUtil::convertMpersToKts(eas) << "kts" << endl;

    //double geoTas = GeoUtil::convertIasToTas(ias, GeoUtil::STD_PRES_HPA, altFt, MathUtil::convertKelvinToCelsius(T));

    cout << "\tOnline TAS: \t" << onlineTas << "kts" << endl;
    cout << "\tAtmos. TAS: \t" << MathUtil::convertMpersToKts(atmosTas) << "kts" << endl;
    //cout << "\tGeo. TAS: \t" << geoTas << "kts" << endl;
}

void testConvertTasToIas(){
    cout << "Test TAS 2:" << endl;
    double mach{};
    double ias = AtmosUtil::convertTasToIas(421, 250, 35269, 34873, 221, mach);

    cout << "\tShould be 250" << endl;
    cout << "\tActually: " << ias << endl;
}

void testCalculatePressureAtAlt(){
    cout << "Test calculatePressureAtAlt 1:" << endl;
    double P = AtmosUtil::calculatePressureAtAlt(10, 0, 1013.25, 273.15 + 25);
    cout << "\tP is " << P << endl;
}

int main(){
    testTrueAirspeed1(260, 35000, -39);
    testConvertTasToIas();
    testCalculatePressureAtAlt();
}