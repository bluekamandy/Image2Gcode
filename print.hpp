#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "layer.hpp"

#ifndef _PRINT_H_
#define _PRINT_H_

class Print
{
public:
    // Default constructor
    Print();

    Print(std::vector<glm::vec3> points);

    float bedWidth;
    float bedHeight;
    float printWidth;
    float printHeight;

    glm::vec3 printOrigin;

    cv::Mat_<cv::Vec3b> image;
    std::vector<unsigned int> imageMatrix;

    std::vector<glm::vec3> points;
    std::vector<int> numLayers;

    std::vector<Layer> layers;

private:
    void makeFrameLayer(float width, float height);
    void processImage();
    void createPoints();
};

#endif