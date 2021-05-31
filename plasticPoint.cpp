#include "plasticPoint.hpp"

// "Color" has an inverse relationship with the extrusion amount.

PlasticPoint::PlasticPoint()
    : point(cv::Point2d(0.0, 0.0)), color(0.0){};

PlasticPoint::PlasticPoint(cv::Point2d point, double color)
    : point(point), color(color){};