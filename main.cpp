/*
 * IMAGE to GCODE
 *
 * by MASOOD
 * 
 * Initiated: 4/27/2021
 * Last updated: 5/26/2021
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
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "slicer.hpp"
#include "layer.hpp"

#define LOG(msg) \
    std::cout << msg << std::endl

using namespace cv;

int main()
{
    // Slicer(float bedWidth, float bedDepth, float maxHeight, float layerHeight, int bedTemp, int nozzleTemp);
    Slicer slicer(220.0f, 220.0f, 250.0f, 0.2f, 70.0f, 200.0f);

    Mat image = imread("../resources/cameraman.jpg");

    Mat resized;

    resize(image, resized, cv::Size2i(128, 128), 0.0, 0.0, INTER_NEAREST);

    // Print(cv::Mat_<cv::Vec3b> image);
    Print print(resized);

    print.processImage();

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