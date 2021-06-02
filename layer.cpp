#include "layer.hpp"

Layer::Layer(){};

void Layer::newSegment(double start, double end, double grayValue)
{
    segmentStart.push_back(start);
    segmentEnd.push_back(end);
    segmentGray.push_back(grayValue);
}