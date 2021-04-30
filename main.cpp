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
#include <opencv2/imgproc.hpp>
#include <glm/vec2.hpp>

#include "slicer.hpp"

#define LOG(msg) \
    std::cout << msg << std::endl

using namespace cv;

Slicer slicer;

cv::Mat DoubleMatFromVec3b(cv::Vec3b in)
{
    cv::Mat mat(3, 1, CV_64FC1);
    mat.at<double>(0, 0) = in[0];
    mat.at<double>(1, 0) = in[1];
    mat.at<double>(2, 0) = in[2];

    return mat;
};

int main()
{
    slicer.test();

    Mat_<Vec3b> image;

    image = imread("../resources/cameraman.jpg");

    int mids = 90; // default is 128.0

    for (auto &pixel : image)
    {
        pixel[0] = pixel[0] < mids ? pixel[0] = 0 : pixel[0] = 255;
        pixel[1] = pixel[1] < mids ? pixel[1] = 0 : pixel[1] = 255;
        pixel[2] = pixel[2] < mids ? pixel[2] = 0 : pixel[2] = 255;
    }

    // Produces a softer mellower color palette.
    // for (auto &pixel : image)
    // {
    //     pixel[0] = (pixel[0] + pixel[1] + pixel[2]) / 3;
    //     pixel[1] = (pixel[0] + pixel[1] + pixel[2]) / 3;
    //     pixel[2] = (pixel[0] + pixel[1] + pixel[2]) / 3;
    // }

    //Dark Black and White Version
    for (auto &pixel : image)
    {
        pixel[0] = std::min({pixel[0], pixel[1], pixel[2]});
        pixel[1] = std::min({pixel[0], pixel[1], pixel[2]});
        pixel[2] = std::min({pixel[0], pixel[1], pixel[2]});
    }

    // Bright Black and White Version
    // for (auto &pixel : image)
    // {
    //     pixel[0] = std::max({pixel[0], pixel[1], pixel[2]});
    //     pixel[1] = std::max({pixel[0], pixel[1], pixel[2]});
    //     pixel[2] = std::max({pixel[0], pixel[1], pixel[2]});
    // }

    int width = image.size().width;
    int height = image.size().height;

    int detail = 100; // how many pixels across and down.

    Mat scaledDown;

    resize(image, scaledDown, Size_<int>(detail, detail), 0, 0, INTER_LINEAR);

    // Sample each pixel from the scaled down image and place them in 2D space using the layer

    resize(scaledDown, image, Size_<int>(width, height), 0, 0, INTER_NEAREST);

    //Mat image2 = DoubleMatFromVec3b(image);
    // Mat image3;

    // cvtColor(image2, image3, COLOR_BGR2HSV, 0);

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    waitKey(0);

    return 0;
}
