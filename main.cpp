/*
 * IMAGE to GCODE
 *
 * by MASOOD
 * 
 * Initiated: 4/27/2021
 * Last updated: 4/28/2021
 * 
 * */

#include <cstdlib>
#include <iostream>
#include <fstream> // Read/write files
#include <string>
#include <config.h>
#include <opencv2/opencv.hpp>
#include <glm/vec2.hpp>

#include "slicer.hpp"

#define LOG(msg) \
    std::cout << msg << std::endl

using namespace cv;

Slicer slicer;

int main()
{
    slicer.test();

    Mat image;
    image = imread("../resources/mandrill.jpg");
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    waitKey(0);

    return 0;
}
