#include "print.hpp"

Print::Print() : bedWidth(220.0f), bedHeight(220.0f), printWidth(128.0f), printHeight(128.0f)
{
    printOrigin = glm::vec3(
        (bedWidth - printWidth) / 2.0f,
        (bedHeight - printHeight) / 2.0f,
        0.0f);
};

Print::Print(cv::Mat_<cv::Vec3b> image) : image(image)
{
}

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

    // Step 1: Harsh split of colors separately
    for (auto &pixel : image)
    {
        pixel[0] = pixel[0] < mids ? pixel[0] = 0 : pixel[0] = 255;
        pixel[1] = pixel[1] < mids ? pixel[1] = 0 : pixel[1] = 255;
        pixel[2] = pixel[2] < mids ? pixel[2] = 0 : pixel[2] = 255;
    }

    // Step 2: Remove color information by taking the maximum value of each channel.
    for (auto &pixel : image)
    {
        pixel[0] = std::max({pixel[0], pixel[1], pixel[2]});
        pixel[1] = std::max({pixel[0], pixel[1], pixel[2]});
        pixel[2] = std::max({pixel[0], pixel[1], pixel[2]});
    }

    //LOG(image);

    cv::Mat RowClone = image.row(image.rows - 1).clone();
    // LOG("RowClone = " << std::endl
    //                   << " " << RowClone << std::endl
    //                   << std::endl);

    int width = image.size().width;
    int height = image.size().height;

    int detail = 100; // how many pixels across and down.

    std::vector<cv::Mat> orderedImageData;

    for (int i = 0; i < image.rows; i++)
    {
        cv::Mat rowClone = image.row(image.rows - 1 - i).clone();

        //LOG("Row #" << i << rowClone);

        orderedImageData.push_back(rowClone);
    }

    for (int y = 0; y < orderedImageData.size(); y++)
    {
        Layer newLayer;
        for (int x = 0; x < orderedImageData[y].cols; x++)
        {
            cv::Mat row = orderedImageData[y];
            if (x < 1)
            {
                newLayer.newSegment(x, row.at<uchar>(x));
            }
            else
            {
                if (row.at<uchar>(x) != row.at<uchar>(x - 1))
                {
                    newLayer.newSegment(x, row.at<uchar>(x));
                }
            }
        }
        layers.push_back(newLayer);

        for (int i = 0; i < newLayer.segmentStart.size(); i++)
        {
            LOG("segmentStart[" << i << "] = " << newLayer.segmentStart[i] << " | segmentGray[" << i << "] = " << newLayer.segmentGray[i]);
        }
    }

    // cv::Mat scaledDown;

    // cv::resize(image, scaledDown, cv::Size_<int>(detail, detail), 0, 0, cv::INTER_LINEAR);

    // cv::resize(scaledDown, image, cv::Size_<int>(width, height), 0, 0, cv::INTER_NEAREST);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey(0);

    /* OTHER IMAGING OPTIONS

    // Harsh split of colors separately
    for (auto &pixel : image)
    {
        pixel[0] = pixel[0] < mids ? pixel[0] = 0 : pixel[0] = 255;
        pixel[1] = pixel[1] < mids ? pixel[1] = 0 : pixel[1] = 255;
        pixel[2] = pixel[2] < mids ? pixel[2] = 0 : pixel[2] = 255;
    }

    // Produces a softer mellower color palette.
    for (auto &pixel : image)
    {
        pixel[0] = (pixel[0] + pixel[1] + pixel[2]) / 3;
        pixel[1] = (pixel[0] + pixel[1] + pixel[2]) / 3;
        pixel[2] = (pixel[0] + pixel[1] + pixel[2]) / 3;
    }

    //Dark Black and White Version
    for (auto &pixel : image)
    {
        pixel[0] = std::min({pixel[0], pixel[1], pixel[2]});
        pixel[1] = std::min({pixel[0], pixel[1], pixel[2]});
        pixel[2] = std::min({pixel[0], pixel[1], pixel[2]});
    }

    // Bright Black and White Version
    for (auto &pixel : image)
    {
        pixel[0] = std::max({pixel[0], pixel[1], pixel[2]});
        pixel[1] = std::max({pixel[0], pixel[1], pixel[2]});
        pixel[2] = std::max({pixel[0], pixel[1], pixel[2]});
    }
    */
}

void Print::createPoints()
{
}