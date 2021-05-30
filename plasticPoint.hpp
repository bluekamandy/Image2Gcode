#pragma once

#include <vector>
#include <opencv2/core/matx.hpp>

#ifndef _PLASTICPOINT_H_
#define _PLASTICPOINT_H_

struct PlasticPoint
{
    glm::vec3 point;
    double color;

    PlasticPoint();

    PlasticPoint(cv::Vec3d point, double color);
};

#endif