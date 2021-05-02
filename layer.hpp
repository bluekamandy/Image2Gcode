#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#ifndef _LAYER_H_
#define _LAYER_H_

struct Layer
{

    std::vector<glm::vec3> points;
    std::vector<int> layerNo;

    Layer();
};

#endif