#include "print.hpp"

Print::Print() : bedWidth(220.0), bedHeight(220.0), printWidth(128.0), printHeight(128.0)
{
    printOrigin = cv::Point2d(
        (bedWidth - printWidth) / 2.0,
        (bedHeight - printHeight) / 2.0);
};

Print::Print(cv::Mat_<cv::Vec3b> image) : image(image)
{
}

// PUBLIC

// PRIVATE

// STEP 1: Process the image into a layer object with all of the values encoded and ordered properly.

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
            //LOG("segmentStart[" << i << "] = " << newLayer.segmentStart[i] << " | segmentGray[" << i << "] = " << newLayer.segmentGray[i]);
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

// STEP 2: Turn that processed image into a series of ordered points along with extrusion values.

void Print::makePoints(double width, double height)
{
    for (int i = 0; i < layers.size(); i++)
    {
        double frame_thickness = 5.0;
        double frame_width = 10.0;

        // Step 1 - Move to 0,0, which is going to be the lower left hand corner of the left frame wall.

        // Step 2 - Recalibrate print origin by setting currentLocation, which is an offset that the frame wall function takes into consideration.

        currentLocation = printOrigin - cv::Point2d(frame_width, frame_width / 2.0);

        // Step 3 - Create left square wall.

        calculateFrameWall(frame_thickness, frame_width);

        // Step 4 - Draw Image layer

        createImagePoints(i);

        // Step 5 - Move currentLocation to the end of the print and do the other side.

        currentLocation = currentLocation + cv::Point2d(frame_width + printWidth, 0.0);

        // Step 6 - Create right square wall.

        calculateFrameWall(frame_thickness, frame_width);
    }
}

void Print::calculateFrameWall(double frame_thickness, double frame_width)
{
    double hDistance = 0.5;
    double color = 0.0;

    cv::Point2d distanceFromOrigin = printOrigin - currentLocation;

    for (int i = 0; i < frame_thickness; i++)
    {
        //LOG("i: " << i);
        PlasticPoint newPoint(cv::Point2d(i * hDistance, i * hDistance) + distanceFromOrigin, color);
        points.push_back(newPoint);
        //LOG(glm::to_string(points.back()));
        newPoint = PlasticPoint(cv::Point2d(frame_width - i * hDistance, i * hDistance) + distanceFromOrigin, color);
        points.push_back(newPoint);
        //LOG(glm::to_string(points.back()));
        newPoint = PlasticPoint(cv::Point2d(frame_width - i * hDistance, frame_width - i * hDistance) + distanceFromOrigin, color);
        points.push_back(newPoint);
        //LOG(glm::to_string(points.back()));
        newPoint = PlasticPoint(cv::Point2d(i * hDistance, frame_width - i * hDistance) + distanceFromOrigin, color);
        points.push_back(newPoint);
        //LOG(glm::to_string(points.back()));
        newPoint = PlasticPoint(cv::Point2d(i * hDistance, i * hDistance + hDistance) + distanceFromOrigin, color);
        points.push_back(newPoint);
        //LOG(glm::to_string(points.back()));
    }
}

void Print::createImagePoints(unsigned int layer_num)
{
    // Move back to print origin and get current color.

    // We also need to take into account the Y value, but THIS IS FOR THE FUTURE.

    for (int j = 0; j < layers[layer_num].segmentStart.size(); j++)
    {
        PlasticPoint newPoint(cv::Point2d(layers[layer_num].segmentStart[j], 0.0), layers[layer_num].segmentGray[j]);
        points.push_back(newPoint);
    }
}