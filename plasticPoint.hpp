#pragma once

#include <vector>
#include <opencv2/core/matx.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#ifndef _PLASTICPOINT_H_
#define _PLASTICPOINT_H_

struct PlasticPoint
{
    cv::Point2d point;
    double color;
    bool printing;

    PlasticPoint();

    PlasticPoint(cv::Point2d point, double color, bool printing);
};

#endif