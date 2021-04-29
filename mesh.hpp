#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#ifndef _NAMEOFCLASS_H_
#define _NAMEOFCLASS_H_

class Mesh
{
public:
    // Default constructor
    Mesh();
    // Specialized constructors go here
    Mesh(std::vector<glm::vec3> points);

    // Any public data/members your class will need should be declared here.
    std::vector<glm::vec3> rawPoints;

    std::vector<glm::vec2> interpPoints;
    std::vector<int> layers;

    // Any methods your class needs go here.

private:
    void makeFrame(float width, float height);
};

#endif