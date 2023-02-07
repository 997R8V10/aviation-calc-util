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

void testNatsToDecimalDegrees(string natsLat, string natsLon) {
    double decLat;
    double decLon;
    GeoUtil::convertNatsToDecimalDegs(natsLat, natsLon, decLat, decLon);
    cout << "Test natsToDecimalDegrees" << endl;
    cout << "\tInput lat\t" << natsLat << endl;
    cout << "\tGot\t" << decLat << endl << endl;
    cout << "\tInput lon\t" << natsLon << endl;
    cout << "\tGot\t" << decLon << endl << endl;
}

void testDecimalToNatsCoords(double decimalLat, double decimalLong) {
    string natsLat;
    string natsLon;
    GeoUtil::convertDecimalDegsToNats(decimalLat, decimalLong, natsLat, natsLon);
    cout << "Test decimalToNatsCoords" << endl;
    cout << "\tInput lat\t" << decimalLat << endl;
    cout << "\tGot\t" << natsLat << endl << endl;
    cout << "\tInput lon\t" << decimalLong << endl;
    cout << "\tGot\t" << natsLon << endl << endl;
}

void testDecimalToVrc(double decimalLat, double decimalLong) {
    string vrcLat;
    string vrcLon;
    GeoUtil::convertDecimalDegsToVrc(decimalLat, decimalLong, vrcLat, vrcLon);
    cout << "Test decimalToVrcCoords" << endl;
    cout << "\tInput lat\t" << decimalLat << endl;
    cout << "\tGot\t" << vrcLat << endl << endl;
    cout << "\tInput lon\t" << decimalLong << endl;
    cout << "\tGot\t" << vrcLon << endl << endl;
}

void testVrcToDecimal(string vrcLat, string vrcLong) {
    double decLat;
    double decLon;
    GeoUtil::convertVrcToDecimalDegs(vrcLat, vrcLong, decLat, decLon);
    cout << "Test vrcToDecimal" << endl;
    cout << "\tInput lat\t" << vrcLat << endl;
    cout << "\tGot\t" << decLat << endl << endl;
    cout << "\tInput lon\t" << vrcLong << endl;
    cout << "\tGot\t" << decLon << endl << endl;
}

int main(){
    testTrueAirspeed1(260, 35000, -39);
    testConvertTasToIas();
    testCalculatePressureAtAlt();
    testNatsToDecimalDegrees("543927N", "0061257W");
    testDecimalToNatsCoords(89.999972, 179.999972);
    testDecimalToVrc(54.6575, -6.21583);
    testVrcToDecimal("N054.39.27.000", "W006.12.57.000");
}