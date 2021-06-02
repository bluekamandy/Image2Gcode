#include "plasticPoint.hpp"

// "Color" has an inverse relationship with the extrusion amount.

PlasticPoint::PlasticPoint()
    : point(cv::Point2d(0.0, 0.0)), color(0.0), printing(true){};

PlasticPoint::PlasticPoint(cv::Point2d point, double color, bool printing)
    : point(point), color(color), printing(printing){};