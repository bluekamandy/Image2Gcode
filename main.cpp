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