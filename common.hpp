#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/matx.hpp>

#ifndef _COMMON_H_
#define _COMMON_H_

// Source. Modified and simplified version of:
// https://www.oreilly.com/library/view/c-cookbook/0596007612/ch04s09.html

std::string join(std::vector<std::string> stringBuffer);

double distance(cv::Point2d point1, cv::Point2d point2);

// Based loosely on Processing.org's map function,
// though I've made the math a bit more clear.

template <class T>
inline T map(T value, T low1, T high1, T low2, T high2)
{
    float slope = (value - low1) / (high1 - low1);
    return slope * (high2 - low2) + low2;
}

#endif