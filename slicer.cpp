#include "slicer.hpp"

// Constructors
Slicer::Slicer()
    : bedWidth(220.0f), bedDepth(220.0f), maxHeight(250.0f), layerHeight(0.2f), bedTemp(80.0f), nozzleTemp(200.0f)
{
}

Slicer::Slicer(double bedWidth, double bedDepth, double maxHeight, double layerHeight, int bedTemp, int nozzleTemp, Print &print)
    : bedWidth(bedWidth), bedDepth(bedDepth), maxHeight(maxHeight), layerHeight(layerHeight), bedTemp(bedTemp), nozzleTemp(nozzleTemp), print(print)
{
}

// Public Methods
void Slicer::test()
{
    std::ofstream myFile("../output/outputTest.txt");

    myFile << "Files can be tricky, but it is fun enough!";

    myFile.close();

    for (int i = 0; i < print.layers.size(); i++)
    {
        for (int j = 0; j < print.layers[i].points.size(); j++)
        {
            LOG("--" << print.layers[i].points[j].point << "--");
        }
    }
}

// Private Methods
void Slicer::makeGcodeInfo(std::vector<std::string> &strBuff)
{
    time_t now = time(0);

    tm *ltm = localtime(&now);

    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(1 + ltm->tm_mon);
    std::string day = std::to_string(ltm->tm_mday);

    strBuff.push_back("; " + year + month + day + '\n');
}

void Slicer::makeGcodeHeatSettings(std::vector<std::string> &strBuff)
{
    strBuff.push_back("M140 S" + std::to_string(bedTemp) + " ; Set Bed Temperature\n");
    strBuff.push_back("M105\n");
    strBuff.push_back("M190 S" + std::to_string(bedTemp) + " ; Wait for Bed Temperature\n");
    strBuff.push_back("M104 S" + std::to_string(nozzleTemp) + " ; Set Nozzle Temperature\n");
    strBuff.push_back("M105\n");
    strBuff.push_back("M109 S" + std::to_string(nozzleTemp) + " ; Wait for Nozzle Temperature\n");
}

void Slicer::makeGcodeStartupSettings(std::vector<std::string> &strBuff)
{
    strBuff.push_back("G28\n");
    strBuff.push_back("M83\n");
    strBuff.push_back("G90\n");
    std::stringstream zOffsetStream;
    zOffsetStream.precision(2);
    zOffsetStream << std::fixed << "G1 F300 Z" << zOffset << " ; set Z-offset\n";
    strBuff.push_back(zOffsetStream.str());
    // strBuff.push_back("G1 X50 E8 F800\n");
    strBuff.push_back(makeGcodePoints(cv::Point2d(10, 10), cv::Point2d(bedWidth - 10, 10)));
    strBuff.push_back(makeGcodePoints(cv::Point2d(bedWidth - 10, 11), cv::Point2d(10, 11)));
}

std::string Slicer::makeRetraction(double amount, double speed, int sign)
{
    std::string message = "";
    if (sign == -1)
    {
        message = " ; Retraction";
    }
    else
    {
        message = " ; Extraction";
    }

    std::stringstream result;

    result << std::fixed << "G1 F" << std::setprecision(0) << speed << " E" << std::setprecision(8) << sign * amount << message << "\n";

    return result.str();
}

std::string Slicer::centerPrint(double printWidth, double printDepth)
{
    cv::Point2d printOrigin;
    printOrigin.x = (bedWidth - printWidth) / 2.0;
    printOrigin.y = (bedDepth - printDepth) / 2.0;

    std::stringstream result;

    result.precision(2);

    result << std::fixed << "G0 F6000 X" << printOrigin.x << " Y" << printOrigin.y << " ; Move to print origin" << '\n'
           << "G92 X0.00 Y0.00 Z0.00 ; Set this point to 0,0,0 in coordinate space\n";

    return result.str();
}

std::string Slicer::makeGcodePoints(cv::Point2d from, cv::Point2d to)
{
    double length = distance(to, from);
    double numerator = nozzleWidth * length * layerHeight;
    double denominator = (1.75 / 2) * (1.75 / 2) * M_PI;
    double e = numerator / denominator;

    e *= extScalar;

    std::stringstream result;

    result << std::fixed << "G1 F1200 X" << std::setprecision(2) << to.x << " Y" << to.y << " E" << std::setprecision(8) << e << '\n';

    return result.str();
}

std::string Slicer::makeGcodeSpeed(cv::Point2d from, cv::Point2d to, double speed)
{
    double length = distance(to, from);
    double numerator = nozzleWidth * length * layerHeight;
    double denominator = (1.75 / 2) * (1.75 / 2) * M_PI;
    double e = numerator / denominator;

    std::stringstream result;

    result << std::fixed << "G1 F" << std::setprecision(0) << speed << " X" << std::setprecision(2) << to.x << " Y" << to.y << " E" << std::setprecision(8) << e << '\n';

    return result.str();
}

std::string Slicer::makeGcode(cv::Point2d to)
{
    std::stringstream result;

    result.precision(2);

    result << "G0 F9000 X" << to.x << " Y" << to.y << '\n';

    return result.str();
}

std::string Slicer::endGcode()
{
    std::stringstream stream;

    stream << "M140 S0 ; Set bed temp to 0\n"
           << "M107 ; Fan off\n"
           << "G91 ;Relative positioning\n"
           << "G1 E-2 F2700 ;Retract a bit\n"
           << "G1 E-2 Z0.2 F2400 ;Retract and raise Z\n"
           << "G1 X5 Y5 F3000 ;Wipe out\n"
           << "G1 Z10 ;Raise Z more\n"
           << '\n'
           << "G90 ;Absolute positioning\n"
           << '\n'
           << "G1 X0 Y220 ;Present print\n"
           << '\n'
           << "M106 S0 ;Turn-off fan\n"
           << "M104 S0 ;Turn-off hotend\n"
           << "M140 S0 ;Turn-off bed\n"
           << '\n'
           << "M84 X Y E ;Disable all steppers but Z\n"
           << '\n'
           << "M82 ;absolute extrusion mode\n"
           << "M104 S0\n"
           << "; Bye Bye! The End.";

    return stream.str();
}

void Slicer::writeFile(std::string str, std::string path)
{
    LOG("Writing to file: "
        << "output.txt");
    std::ofstream myFile("output.txt");
    myFile << str;
    myFile.close();
}

void Slicer::apply()
{
    std::vector<std::string> stringBuffer;

    makeGcodeInfo(stringBuffer);
    makeGcodeHeatSettings(stringBuffer);
    makeGcodeStartupSettings(stringBuffer);
    stringBuffer.push_back(makeRetraction(retAmount, retSpeed, -1));
    stringBuffer.push_back(centerPrint(128.0f, 128.0f));

    double currentHeight = 0.0;

    // print.layers.size() is the number of print.layers in the 3d print.
    // i * layerHeight should be the z position of the printer.

    for (int i = 0; i < print.layers.size(); i++)
    {
        std::vector<PlasticPoint> plasticPoints = print.layers[i].points;

        if (plasticPoints.size() != 0)
        {
            currentHeight = i * layerHeight;
            stringBuffer.push_back("G1 F300 Z" + std::to_string(currentHeight) + "\n");
            stringBuffer.push_back(makeGcode(plasticPoints[0].point));
            stringBuffer.push_back(makeRetraction(retAmount, retSpeed, 1));

            for (int j = 0; j < plasticPoints.size(); j++)
            {
                if (j == 0)
                {
                    stringBuffer.push_back(makeGcode(plasticPoints[j].point));
                    continue;
                }

                stringBuffer.push_back(makeGcodeSpeed(plasticPoints[j - 1].point, plasticPoints[j].point, printSpeed));
            }

            stringBuffer.push_back(makeRetraction(retAmount, retSpeed, -1));
        }
    }

    stringBuffer.push_back(makeRetraction(retAmount, retSpeed, -1));
    currentHeight += 10;

    std::stringstream precisionChange;

    precisionChange.precision(3);

    precisionChange << "G1 Z" << currentHeight << "\n";

    stringBuffer.push_back(precisionChange.str());

    stringBuffer.push_back(endGcode());

    // END -- Now join string and write it to a file.

    std::string stringBufferJoined = join(stringBuffer);

    LOG(stringBufferJoined);

    writeFile(stringBufferJoined, filePath);
}
