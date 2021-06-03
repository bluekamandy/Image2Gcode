#include "common.hpp"

std::string join(std::vector<std::string> stringBuffer)
{
    std::string s = "";

    for (std::vector<std::string>::const_iterator i = stringBuffer.begin();
         i != stringBuffer.end(); ++i)
    {
        s += *i;
    }

    return s;
}

// Several sources helped me with this:
// https://cppsecrets.com/users/168511510411111711412197115105110104975764103109971051084699111109/C00-OpenCV-cvmagnitude.php
// https://docs.opencv.org/3.4/d2/de8/group__core__array.html#ga6d3b097586bca4409873d64a90fe64c3
// https://stackoverflow.com/questions/8269605/opencv-c-distance-between-two-points

/*

Point difference=Point1-Point2;
double distance =sqrt( difference.ddot(difference));

*/

double distance(cv::Point2d point1, cv::Point2d point2)
{
    cv::Point2d difference = point1 - point2;

    double distance = sqrt(difference.ddot(difference));

    return distance;
}