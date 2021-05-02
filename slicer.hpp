/*
 * BASIC SLICER
 *
 * by MASOOD
 * adapted from Python code by Jennifer Jacobs
 * for her Computational Fabrication course in
 * UCSB's Media Arts & Technology department..
 * 
 * Initiated: 4/27/2021
 * Last updated: 4/30/2021
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
#include "mesh.hpp"
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
    Slicer(float bedWidth, float bedDepth, float maxHeight, float layerHeight, int bedTemp, int nozzleTemp);

    // Any public data/members your class will need should be declared here.
    std::string output;
    std::string filePath;

    Mesh mesh;
    std::vector<Layer> layers;

    float bedWidth;
    float bedDepth;
    float maxHeight;

    int bedTemp;
    int nozzleTemp;

    float extScalar = 5.0f;
    float zOffset = 0.2f;

    float layerHeight;
    float nozzleWidth = 0.4f;
    float retAmount = 6.0f;
    float retSpeed = 1200.0f;
    float printSpeed = 500.0f;
    float printSpeedHigh = 1000.0f;

    // Any methods your class needs go here.
    void apply();
    void test();

private:
    void makeGcodeInfo(std::vector<std::string> &strBuff);
    void makeGcodeHeatSettings(std::vector<std::string> &strBuff);
    void makeGcodeStartupSettings(std::vector<std::string> &strBuff);

    std::string makeRetraction(float amount, float speed, int sign);
    std::string centerPrint(float printWidth, float);
    std::string makeGcodePoints(glm::vec2 from, glm::vec2 to);
    std::string makeGcodeSpeed(glm::vec2 from, glm::vec2 to, float speed);
    std::string makeGcode(glm::vec2 to);
    std::string endGcode();

    void writeFile(std::string str, std::string path);
};

#endif