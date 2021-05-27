#include "layer.hpp"

Layer::Layer(){};

void Layer::newSegment(unsigned int start, unsigned int grayValue)
{
    segmentStart.push_back(start);
    segmentGray.push_back(grayValue);
}