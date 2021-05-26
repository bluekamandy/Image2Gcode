#include "print.hpp"

Print::Print() : bedWidth(220.0f), bedHeight(220.0f), printWidth(128.0f), printHeight(128.0f)
{
    printOrigin = glm::vec3(
        (bedWidth - printWidth) / 2.0f,
        (bedHeight - printHeight) / 2.0f,
        0.0f);
};

Print::Print(std::vector<glm::vec3> points) : points(points) {}

// PUBLIC

// PRIVATE

void Print::makeFrameLayer(float width, float height)
{
    //Left-UL
    //Left-UR
    //Left-LL
    //Left-LR

    //Right-UL
    //Right-UR
    //Right-LL
    //Right-LR
}

void Print::processImage()
{
    int mids = 90; // default is 128.0

    for (auto &pixel : image)
    {
        pixel[0] = pixel[0] < mids ? pixel[0] = 0 : pixel[0] = 255;
        pixel[1] = pixel[1] < mids ? pixel[1] = 0 : pixel[1] = 255;
        pixel[2] = pixel[2] < mids ? pixel[2] = 0 : pixel[2] = 255;
    }

    int width = image.size().width;
    int height = image.size().height;

    int detail = 100; // how many pixels across and down.

    cv::Mat scaledDown;

    cv::resize(image, scaledDown, cv::Size_<int>(detail, detail), 0, 0, cv::INTER_LINEAR);

    cv::resize(scaledDown, image, cv::Size_<int>(width, height), 0, 0, cv::INTER_NEAREST);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey(0);
}

void Print::createPoints()
{
}