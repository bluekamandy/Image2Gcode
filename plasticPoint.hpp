#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#ifndef _PLASTICPOINT_H_
#define _PLASTICPOINT_H_

struct PlasticPoint
{
    glm::vec3 point;
    double color;

    PlasticPoint();

    PlasticPoint(glm::vec3 point, double color);
};

#endif