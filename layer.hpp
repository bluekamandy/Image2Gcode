#pragma once

#include <vector>
#include <opencv2/core/matx.hpp>
#include "plasticPoint.hpp"

#ifndef _LAYER_H_
#define _LAYER_H_

struct Layer
{
    double length;

    std::vector<double> segmentStart; // These are the segments of a single color.
                                      // Imagine a line that starts at 0 and ends at length
    std::vector<double> segmentEnd;   // We need to know when they end also so that we have a complete image.
    std::vector<double> segmentGray;  // Each segmentStart corresponds to a gray value.
    std::vector<PlasticPoint> points;

    Layer();

    void newSegment(double start, double end, double grayValue);
};

#endif