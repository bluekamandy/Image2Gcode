/*
 * IMAGE to GCODE
 *
 * by MASOOD KAMANDY
 * 
 * Initiated: 4/27/2021
 * Last updated: 6/10/2021
 * 
 * */

#include <cstdlib>
#include <iostream>
#include <fstream> // Read/write files
#include <string>
#include <sstream>
#include <config.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/matx.hpp>

#include "slicer.hpp"
#include "layer.hpp"

#define LOG(msg) \
    std::cout << msg << std::endl

using namespace cv;

int main()
{
    // Load the image. The image should be a square for now.
    Mat image = imread("../resources/harvey_milk.jpg");

    // Mat image = imread("../resources/cameraman.jpg", IMREAD_GRAYSCALE);
    // Mat thresh_image;

    // double thresh = 0;
    // double maxValue = 255;

    // threshold(image, thresh_image, thresh, maxValue, THRESH_BINARY);

    Mat thresh_image;

    double thresh = 127;
    double maxValue = 255;

    threshold(image, thresh_image, thresh, maxValue, THRESH_BINARY);

    // Resize the image to 128 x 128.
    Mat resized;
    resize(thresh_image, resized, cv::Size(128, 128), 0.0, 0.0, INTER_NEAREST);
    // resize(thresh_image, resized, cv::Size2i(128, 128), 0.0, 0.0, INTER_NEAREST);

    double bedWidth = 220.0;
    double bedDepth = 220.0;
    double printWidth = resized.size().width;
    double printDepth = 10.0;
    double maxHeight = 250.0;
    double layerHeight = 0.33;
    double bedTemp = 65.0;
    double nozzleTemp = 210.0;

    // Print::Print(cv::Mat_<cv::Vec3b> image, double bedWidth, double bedDepth, double printWidth, double printDepth)
    // : image(image), bedWidth(bedWidth), bedDepth(bedDepth), printWidth(printWidth), printDepth(printDepth)
    Print print(resized, bedWidth, bedDepth, printWidth, printDepth);

    // Process the image.
    print.process();
    print.test();

    // Slicer(float bedWidth, float bedDepth, float maxHeight, float layerHeight, int bedTemp, int nozzleTemp);
    Slicer slicer(bedWidth, bedDepth, maxHeight, layerHeight, bedTemp, nozzleTemp, print);

    slicer.filePath = "../output/";
    slicer.output = "output.gcode";

    slicer.test();

    slicer.apply();

    LOG("DISTANCE TEST IS: " << distance(cv::Point2d(0, 0), cv::Point2d(23, 40)));

    LOG("Image has: " << print.image.rows << " rows and " << print.image.cols << " columns.");

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", print.image);

    cv::waitKey(0);

    //double distance(cv::Point2d point1, cv::Point2d point2)

    return 0;
}

//--
// Sample each pixel from the scaled down image and place them in 2D space using the layer
//--

//--
// DRAW SQUARE
//--

// CENTER BUILD AREA
// Build area for this will be pixels/2
// Since images are 256x256, our image will be 128x128

// To center we need to go to the origin of the print and make all points relative to that.

// printOrigin.x = (bedWidth - printWidth) / 2;
// printOrigin.y = (bedHeight - printHeight) / 2;

// Since our bedWidth and bedHeight both == 220mm
// and our printWidth and Height both == 128mm
// we can say:

// printOrigin.x = (220 - 128) / 2.0;
// printOrigin.y = (220 - 128) / 2.0;

// Set the origin to (printOrigin.x, printOrigin.y)

// G92 X0 Y0 Z0

// The square border will be 5 mm thick

// xmax = maximum print width
// ymax = maximum print height

//0,0
//Xmax, 0
//Xmax, Ymax
//0, ymax
//0,1

//1,1
//xmax -1, 1
//xmax - 1, ymax - 1
//1, ymax-1
//1,2

//  std::vector<Layer> print;

//     Layer firstLayer;

//     print.push_back(firstLayer);

//     float hDistance = 0.5f;

//     for (int i = 0; i < 5; i++)
//     {
//         LOG("i: " << i);
//         print[0].points.push_back(glm::vec3(i * hDistance, i * hDistance, 0));
//         LOG(glm::to_string(print[0].points.back()));
//         print[0].points.push_back(glm::vec3(printWidth - i * hDistance, i * hDistance, 0));
//         LOG(glm::to_string(print[0].points.back()));
//         print[0].points.push_back(glm::vec3(printWidth - i * hDistance, printHeight - i * hDistance, 0));
//         LOG(glm::to_string(print[0].points.back()));
//         print[0].points.push_back(glm::vec3(i * hDistance, printHeight - i * hDistance, 0));
//         LOG(glm::to_string(print[0].points.back()));
//         print[0].points.push_back(glm::vec3(i * hDistance, i * hDistance + hDistance, 0));
//         LOG(glm::to_string(print[0].points.back()));
//     }

//     slicer.layers = print;
//     slicer.filePath = "output/";
//     slicer.apply();