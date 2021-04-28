#pragma once

#include <fstream>
#include <string>

#ifndef _SLICER_H_
#define _SLICER_H_

class Slicer
{
public:
    // Default constructor
    Slicer();
    Slicer(float layerHeight);

    // Any public data/members your class will need should be declared here.
    std::string output;

    float layerHeight;
    float nozzleWidth = 0.4;
    float retAmount = 6.0;
    float retSpeed = 1200;
    float printSpeed = 500;
    float printSpeedHigh = 1000;

    // Any methods your class needs go here.

private:
    void privateMethod();
};

#endif