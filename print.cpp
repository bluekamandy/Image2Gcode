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
    // REPLACED THIS WITH THRESHOLD FUNCTION IN MAIN

    // LOG("Processing Image");

    // int mids = 90; // default is 128.0

    // // Step 1: Harsh split of colors separately
    // for (auto &pixel : image)
    // {
    //     pixel[0] = pixel[0] < mids ? pixel[0] = 0 : pixel[0] = 255;
    // }

    // // Step 2: Remove color information by taking the maximum value of each channel.
    // for (auto &pixel : image)
    // {
    //     pixel[0] = std::max({pixel[0], pixel[1], pixel[2]});
    // }

    //LOG(image);

    // cv::Mat RowClone = image.row(image.rows - 1).clone();
    // LOG("RowClone = " << std::endl
    //                   << " " << RowClone << std::endl
    //                   << std::endl);

    // int width = image.size().width;
    // int height = image.size().height;

    // WE'RE MOVING AWAY FROM THE ROW CLONE REVERSE METHOD AND JUST FLIPPING.

    // // Reverse the order of the image rows.
    // for (int i = 0; i < image.rows; i++)
    // {
    //     cv::Mat rowClone = image.row(image.rows - 1 - i).clone();

    //     LOG("Row #" << i << rowClone);

    //     orderedImageData.push_back(rowClone);
    // }

    // Let's make a set of layers the size of the image.

    cv::flip(image, flipped, 0);

    for (int i = 0; i < flipped.rows; i++)
    {
        Layer newLayer;
        layers.push_back(newLayer);
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

        // Step 2 - currentLocation, which is an offset that the frame wall function takes into consideration.

        currentLocation = printOrigin - cv::Point2d(frame_width, frame_width / 2.0);

        // Step 3 - Create left square wall.

        calculateFrameWall(layer_num, frame_thickness, frame_width);

        // Step 1 - Create image matrix that will use the image data to print.

        createImageMatrixPoints(layer_num);

        // Step 4 - Move currentLocation to the end of the print and do the other side.

        currentLocation = currentLocation + cv::Point2d(frame_width + printWidth, 0.0);

        // Step 5 - Create right square wall.

        calculateFrameWall(layer_num, frame_thickness, frame_width);

        currentLocation = printOrigin;
    }
}

void Print::createImageMatrixPoints(unsigned int layer_num)
{
    // In 3D printing, pixels are not discrete objects. Realizing this was an important shift for me.
    // We stretch a single pixel over space. This means there is a scaffold that holds the image.
    // Here are 3 pixels in 3D printing space:
    // |>-<|>-<|>-<|
    // This means that every pixel is actually 3 segments.
    // - The FIRST segement is a WARP |> has an extrusion value of 1.0. This distance is parameterized. I'm starting with 0.2.
    // - The SECOND segment - has a mapped extrusion value that under-extrudes depending on the color value of the pixel.
    // - The THIRD segement is a WARP <| is another segement that has an extrusion value of 1.0 to "hold" the image and secure the layers of print.

    // NOTE: Maybe a better visualization is =---= =---= =---= =---= =---= =---= =---=, where each set of =-= has a single "pixel" that is surrounded by a warp.

    // If the tolerance is 0 -> =-=

    // First let's create all of the points for the entire image.

    // We'll use a weaving metaphor.
    // NOTE: pixel + 0.2 is the warp
    // i + 0.2 + 0.6 is the weft
    // and then the warp is what's left over (i + 0.2 + 0.6 + 0.2)
    double warp = 0.2;
    double weft = 0.6;

    PlasticPoint newPoint(cv::Point2d(0.0, 0.0), 0.0, false);
    layers[layer_num].points.push_back(newPoint);

    for (int pixel = 0; pixel < flipped.cols; pixel++) // Go through each pixel in the row.
                                                       // By changing the increment, you can make the secquences between the warps longer. It doesn't work well enough to make a difference in the actual print though, so I think restructuring the actual warp/weft structure below is a better strategy. Maybe average a set number of pixels?
    {
        LOG("INSIDE FOR LOOP: " << pixel);

        for (int pixel_segment = 0; pixel_segment < 3; pixel_segment++) // There are 3 segments per pixel.
        {
            switch (pixel_segment)
            {
            case WARP_LEFT: // WARP (create an enum)
                // Reminder: PlasticPoint(cv::Point2d point, double color, bool printing);
                // 255 is WHITE is mapped to full extrusion.
                newPoint = PlasticPoint(cv::Point2d(i, 0.0), 255, true);
                LOG("WARP - Point: (" << pixel << ", 0.0) Color: " << 255);
                break;
            case WEFT: // WEFT (Tolerance on either side? )
                // Wishful thinking.
                createImageSegment(image, tolerance, x, y);
                // Here we are just pitting the ACTUAL GRAY VALUE as color
                newPoint = PlasticPoint(cv::Point2d(pixel + warp, 0.0), (double)flipped.at<uchar>(layer_num, pixel * 3), true);
                LOG("WEFT - Point: (" << pixel + warp << ", 0.0) Color: " << (double)flipped.at<uchar>(layer_num, i * 3));
                break;
            case WARP_RIGHT: // WARP
                // 255 is WHITE is mapped to full extrusion.
                newPoint = PlasticPoint(cv::Point2d(i + warp + weft, 0.0), 255, true);
                LOG("WARP - Point: (" << i << ", 0.0) Color: " << 255);
                break;
            }
            layers[layer_num].points.push_back(newPoint);
        }
    }

    newPoint = PlasticPoint(cv::Point2d((double)flipped.cols, 0.0), 0.0, false);
    layers[layer_num].points.push_back(newPoint);
}

void Print::calculateFrameWall(unsigned int layer_num, unsigned int frame_thickness, unsigned int frame_width)
{
    double hDistance = 0.5;
    double color = 0.0;

    cv::Point2d distanceFromOrigin = currentLocation - printOrigin;

    for (int i = 0; i < frame_thickness; i++)
    {
        LOG("i: " << i);
        // Get to starting point with no printing.
        PlasticPoint newPoint(cv::Point2d(i * hDistance, i * hDistance) + distanceFromOrigin, color, false);
        layers[layer_num].points.push_back(newPoint);

        newPoint = PlasticPoint(cv::Point2d(i * hDistance, i * hDistance) + distanceFromOrigin, color, true);
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

        newPoint = PlasticPoint(cv::Point2d(i * hDistance, i * hDistance + hDistance) + distanceFromOrigin, color, true);
        layers[layer_num].points.push_back(newPoint);
        LOG("x = " << layers[layer_num].points.back().point.x << " and y = " << layers[layer_num].points.back().point.y);

        newPoint = PlasticPoint(cv::Point2d(i * hDistance, i * hDistance + hDistance) + distanceFromOrigin, color, false);
        layers[layer_num].points.push_back(newPoint);
    }
}
