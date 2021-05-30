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
    // Default constructor
    Print();

    Print(cv::Mat_<cv::Vec3b> image);

    double bedWidth;
    double bedHeight;
    double printWidth;
    double printHeight;

    cv::Point2d printOrigin;
    cv::Point2d currentLocation;

    cv::Mat_<cv::Vec3b> image;
    std::vector<unsigned int> imageMatrix;

    std::vector<PlasticPoint> points;
    std::vector<int> numLayers;

    std::vector<Layer> layers;

    void processImage();

private:
    // void makePlatform(unsigned int width, unsigned int height);
    void calculateFrameWall(double frame_thickness, double frame_width);
    void makePoints(double width, double height);
    void makeImageLayer();
    void createImagePoints(unsigned int layer_num);
};

#endif