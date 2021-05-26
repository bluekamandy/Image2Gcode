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

    Slicer slicer(220.0f, 220.0f, 250.0f, 0.2f, 70.0f, 200.0f);

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

    Print print;

    // print.push_back(firstLayer);

    // float hDistance = 0.5f;

    // for (int i = 0; i < 5; i++)
    // {
    //     LOG("i: " << i);
    //     print[0].points.push_back(glm::vec3(i * hDistance, i * hDistance, 0));
    //     LOG(glm::to_string(print[0].points.back()));
    //     print[0].points.push_back(glm::vec3(printWidth - i * hDistance, i * hDistance, 0));
    //     LOG(glm::to_string(print[0].points.back()));
    //     print[0].points.push_back(glm::vec3(printWidth - i * hDistance, printHeight - i * hDistance, 0));
    //     LOG(glm::to_string(print[0].points.back()));
    //     print[0].points.push_back(glm::vec3(i * hDistance, printHeight - i * hDistance, 0));
    //     LOG(glm::to_string(print[0].points.back()));
    //     print[0].points.push_back(glm::vec3(i * hDistance, i * hDistance + hDistance, 0));
    //     LOG(glm::to_string(print[0].points.back()));
    // }

    // slicer.layers = print;
    // slicer.filePath = "output/";
    // slicer.apply();

    // for (int i = 0; i < print[0].points.size(); i++)
    // {
    //     LOG(glm::to_string(print[0].points[i]));
    // }

    //2,2

    resize(scaledDown, image, Size_<int>(width, height), 0, 0, INTER_NEAREST);

    //Mat image2 = DoubleMatFromVec3b(image);
    // Mat image3;

    // cvtColor(image2, image3, COLOR_BGR2HSV, 0);

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    waitKey(0);

    return 0;
}
