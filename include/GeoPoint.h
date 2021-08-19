//
// Created by pshivaraman on 2021-08-19.
//

#ifndef AVIATION_SIM_UTIL_GEOPOINT_H
#define AVIATION_SIM_UTIL_GEOPOINT_H

class GeoPoint {
private:
    double lat;
    double lon;
    double alt;

public:
    // Constructors
    GeoPoint(GeoPoint &point);
    GeoPoint(double lat, double lon, double alt);
    GeoPoint(double lat, double lon);

    // Methods
    void moveByM(double bearing, double distance);
    void moveByNMi(double bearing, double distance);
    static double flatDistanceM(GeoPoint &point1, GeoPoint &point2);
    static double flatDistanceNMi(GeoPoint &point1, GeoPoint &point2);

    // Getters/Setters
    double getLat();
    void setLat(double lat);
    double getLon();
    void setLon(double lon);
    double getAlt();
    void setAlt(double alt);
};

#endif //AVIATION_SIM_UTIL_GEOPOINT_H
