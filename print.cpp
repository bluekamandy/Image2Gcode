#include "print.hpp"

Print::Print(cv::Mat_<cv::Vec3b> image, double bedWidth, double bedDepth, double printWidth, double printDepth)
    : image(image), bedWidth(bedWidth), bedDepth(bedDepth), printWidth(printWidth), printDepth(printDepth)
{
    printWidth = image.size().width;

    frame_width = printDepth;

    printOrigin = cv::Point2d(
        (bedWidth - printWidth) / 2.0, /* Take width of print into account */
        bedDepth / 2.0);               /* Exactly in the middle */
}

// PUBLIC
void Print::process()
{
    processImage();
    makePoints();
}

void Print::test()
{
    LOG("Printing all points");
    LOG("layers.size() = " << layers.size());

    for (int i = 0; i < layers.size(); i++)
    {
        LOG("layers[" << i << "].points.size() = " << layers[i].points.size());

        for (int j = 0; j < layers[i].points.size(); j++)
        {
            LOG("X = " << layers[i].points[j].point.x << " and Y = " << layers[i].points[j].point.y);
        }
    }
}

// STEP 1: Process the image into a layer object with all of the values encoded and ordered properly.

void Print::processImage()
{
    LOG("Processing Image");

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

    // cv::Mat RowClone = image.row(image.rows - 1).clone();
    // LOG("RowClone = " << std::endl
    //                   << " " << RowClone << std::endl
    //                   << std::endl);

    int width = image.size().width;
    int height = image.size().height;

    std::vector<cv::Mat> orderedImageData;

    // Reverse the order of the image rows.
    for (int i = 0; i < image.rows; i++)
    {
        cv::Mat rowClone = image.row(image.rows - 1 - i).clone();

        //LOG("Row #" << i << rowClone);

        orderedImageData.push_back(rowClone);
    }

    // Create layers with the different color data.

    // Each row contains 128 bins.
    // Each of the bins either has 255 or 0 in them.
    // If you're at element 0, set segmentStart to 0.0
    // If you're not at elebemnt 0, check if the prior color was different.
    // If the color was different than the present element, push a segment.
    // Start a new segment.

    for (int y = 0; y < orderedImageData.size(); y++)
    {
        Layer newLayer;

        for (int x = 0; x < orderedImageData[y].cols; x++)
        {
            double segmentStart;
            double segmentEnd;

            cv::Mat &row = orderedImageData[y];

            if (x == 0)
            {
                segmentStart = 0.0;
            }
            else
            {
                if (row.at<uchar>(x) != row.at<uchar>(x - 1))
                {
                    segmentEnd = x;
                    newLayer.newSegment(segmentStart, segmentEnd, row.at<uchar>(x));
                    segmentStart = x;
                }
                else if (x == orderedImageData[y].cols - 1)
                {
                    segmentEnd = x;
                    newLayer.newSegment(segmentStart, segmentEnd, row.at<uchar>(x));
                }
            }
        }

        layers.push_back(newLayer);

        for (int i = 0; i < newLayer.segmentStart.size(); i++)
        {
            LOG("segmentStart[" << i << "] = " << newLayer.segmentStart[i] << " | segmentGray[" << i << "] = " << newLayer.segmentGray[i]);
        }

        for (int i = 0; i < layers.size(); i++)
        {
            for (int j = 0; j < layers[i].segmentStart.size(); j++)
            {
                LOG("layer[" << i << "].segmentStart[" << j << "] = " << layers[i].segmentStart[j] << " | layer[" << i << "].segmentGray[" << j << "] = " << layers[i].segmentGray[j]);
            }
        }
    }
}

// STEP 2: Turn that processed image into a series of ordered points along with extrusion values.
// It was suggested that we print the image first and then the posts.

void Print::makePoints()
{
    LOG("Making points from the layer object.");

    for (int layer_num = 0; layer_num < layers.size(); layer_num++)
    {
        // These are hard coded for now.
        // If when we're printing, we find a need, we may create an interface for them.
        unsigned int frame_thickness = 5.0;

        // Step 1 - Create image points. The image always has to be printed first before

        createImagePoints(layer_num);

        // Step 2 - currentLocation, which is an offset that the frame wall function takes into consideration.

        currentLocation = printOrigin - cv::Point2d(frame_width, frame_width / 2.0);

        // Step 3 - Create left square wall.

        calculateFrameWall(layer_num, frame_thickness, frame_width);

        // Step 4 - Move currentLocation to the end of the print and do the other side.

        currentLocation = currentLocation + cv::Point2d(frame_width + printWidth, 0.0);

        // Step 5 - Create right square wall.

        calculateFrameWall(layer_num, frame_thickness, frame_width);

        currentLocation = printOrigin;
    }
}

void Print::createImagePoints(unsigned int layer_num)
{
    //LOG("Creating image points for layer number: " << layer_num);

    for (int i = 0; i < layers[layer_num].segmentStart.size(); i++)
    {
        PlasticPoint newPoint = PlasticPoint(cv::Point2d(layers[layer_num].segmentStart[i], 0.0), layers[layer_num].segmentGray[i], true);
        layers[layer_num].points.push_back(newPoint);
        //LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);
    }

    // Now we need to end the segment at the end of the print.
    PlasticPoint endPoint = PlasticPoint(
        cv::Point2d(printWidth, 0.0),
        layers[layer_num].segmentGray[layers[layer_num].segmentGray.size() - 1], false);
    layers[layer_num].points.push_back(endPoint);
    //LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);
}

void Print::calculateFrameWall(unsigned int layer_num, unsigned int frame_thickness, unsigned int frame_width)
{
    double hDistance = 0.5;
    double color = 0.0;

    cv::Point2d distanceFromOrigin = currentLocation - printOrigin;

    for (int i = 0; i < frame_thickness; i++)
    {
        LOG("i: " << i);
        PlasticPoint newPoint(cv::Point2d(i * hDistance, i * hDistance) + distanceFromOrigin, color, true);
        layers[layer_num].points.push_back(newPoint);
        LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);

        newPoint = PlasticPoint(cv::Point2d(frame_width - i * hDistance, i * hDistance) + distanceFromOrigin, color, true);
        layers[layer_num].points.push_back(newPoint);
        LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);

        newPoint = PlasticPoint(cv::Point2d(frame_width - i * hDistance, frame_width - i * hDistance) + distanceFromOrigin, color, true);
        layers[layer_num].points.push_back(newPoint);
        LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);

        newPoint = PlasticPoint(cv::Point2d(i * hDistance, frame_width - i * hDistance) + distanceFromOrigin, color, true);
        layers[layer_num].points.push_back(newPoint);
        LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);

        newPoint = PlasticPoint(cv::Point2d(i * hDistance, i * hDistance + hDistance) + distanceFromOrigin, color, false);
        layers[layer_num].points.push_back(newPoint);
        LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);
    }
}
