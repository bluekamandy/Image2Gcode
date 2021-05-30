#include "layer.hpp"

Layer::Layer(){};

void Layer::newSegment(double start, double grayValue)
{
    segmentStart.push_back(start);
    segmentGray.push_back(grayValue);
}