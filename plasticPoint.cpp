#include "plasticPoint.hpp"

// "Color" has an inverse relationship with the extrusion amount.

PlasticPoint::PlasticPoint() : point(cv::Point3d(0.0, 0.0, 0.0)), color(0.0){};

PlasticPoint::PlasticPoint(cv::Point3d point, double color){

};