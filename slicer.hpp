/*
 * BASIC SLICER
 *
 * by MASOOD
 * adapted from Python code by Jennifer Jacobs
 * for her Computational Fabrication course at
 * UCSB's Media Arts & Technology course.
 * 
 * Initiated: 4/27/2021
 * Last updated: 4/28/2021
 * 
 * */

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <glm/geometric.hpp>

#include "mesh.hpp"

#ifndef _SLICER_H_
#define _SLICER_H_

class Slicer
{
public:
    // Default constructor
    Slicer();
    Slicer(float layerHeight, int bedTemp, int nozzleTemp);

    // Any public data/members your class will need should be declared here.
    std::string output;

    Mesh mesh;

    int bedTemp;
    int nozzleTemp;

    float layerHeight;
    float nozzleWidth = 0.4;
    float retAmount = 6.0;
    float retSpeed = 1200;
    float printSpeed = 500;
    float printSpeedHigh = 1000;

    // Any methods your class needs go here.
    void apply();
    void test();

private:
    void makeGcodeInfo(std::vector<std::string> &strBuff);
    void makeGcodeHeatSettings(std::vector<std::string> &strBuff);
    void makeGcodeStartupSettings(std::vector<std::string> &strBuff);

    void writeFile(std::string str, std::string path);

    std::string makeRetraction(float amount, float speed, int sign);
    std::string makeGcodePoints(glm::vec2 from, glm::vec2 to);
    std::string makeGcodeSpeed(glm::vec2 from, glm::vec2 to, float speed);
    std::string makeGcode(glm::vec2 to);
};

#endif