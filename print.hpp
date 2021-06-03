#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "layer.hpp"
#include "plasticPoint.hpp"

#define LOG(msg) \
    std::cout << msg << std::endl

#ifndef _PRINT_H_
#define _PRINT_H_

class Print
{
public:
    Print(cv::Mat_<cv::Vec3b> image, double bedWidth, double bedDepth, double printWidth, double printDepth);

    double bedWidth;
    double bedDepth;
    double printWidth;
    double printDepth;
    unsigned int frame_width;

    cv::Point2d printOrigin;
    cv::Point2d currentLocation;

    cv::Mat_<cv::Vec3b> image;
    cv::Mat_<cv::Vec3b> flipped;

    std::vector<PlasticPoint> points;

    std::vector<Layer> layers;

    void process();
    void test();

private:
    Print();
    // At some point, depending on tests, it may be useful to write a platform-creation function for the plastic to be more stable.

    // Called by process();
    void processImage();
    void makePoints();

    // Called by the functions above.
    void calculateFrameWall(unsigned int layer_num, unsigned int frame_thickness, unsigned int frame_width);
    void createImageMatrixPoints(unsigned int layer_num);
};

#endif