#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#ifndef _LAYER_H_
#define _LAYER_H_

struct Layer
{
    float length;

    std::vector<float> segmentStart;       // These are the segments of a single color.
                                           // Imagine a line that starts at 0 and ends at length
    std::vector<unsigned int> segmentGray; // Each segmentStart corresponds to a gray value.
    std::vector<int> layerNo;

    Layer();

    void newSegment(glm::vec3 start, unsigned int grayValue);
};

#endif