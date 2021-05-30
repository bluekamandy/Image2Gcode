/*
 * BASIC SLICER
 *
 * by MASOOD
 * adapted from Python code by Jennifer Jacobs
 * for her Computational Fabrication course in
 * UCSB's Media Arts & Technology department..
 * 
 * Initiated: 4/27/2021
 * Last updated: 5/29/2021
 * 
 * */

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <glm/geometric.hpp>

#include "common.hpp"
#include "print.hpp"
#include "layer.hpp"

#define LOG(msg) \
    std::cout << msg << std::endl

#ifndef _SLICER_H_
#define _SLICER_H_

class Slicer
{
public:
    // Default constructor
    Slicer();
    Slicer(double bedWidth, double bedDepth, double maxHeight, double layerHeight, int bedTemp, int nozzleTemp);

    // Any public data/members your class will need should be declared here.
    std::string output;
    std::string filePath;

    Print print;
    std::vector<Layer> layers;

    double bedWidth;
    double bedDepth;
    double maxHeight;

    int bedTemp;
    int nozzleTemp;

    double extScalar = 5.0f;
    double zOffset = 0.2f;

    double layerHeight;
    double nozzleWidth = 0.4f;
    double retAmount = 6.0f;
    double retSpeed = 1200.0f;
    double printSpeed = 500.0f;
    double printSpeedHigh = 1000.0f;

    // Any methods your class needs go here.
    void apply();
    void test();

private:
    void makeGcodeInfo(std::vector<std::string> &strBuff);
    void makeGcodeHeatSettings(std::vector<std::string> &strBuff);
    void makeGcodeStartupSettings(std::vector<std::string> &strBuff);

    std::string makeRetraction(double amount, double speed, int sign);
    std::string centerPrint(double printWidth, double);
    std::string makeGcodePoints(glm::vec2 from, glm::vec2 to);
    std::string makeGcodeSpeed(glm::vec2 from, glm::vec2 to, double speed);
    std::string makeGcode(glm::vec2 to);
    std::string endGcode();

    void writeFile(std::string str, std::string path);
};

#endif