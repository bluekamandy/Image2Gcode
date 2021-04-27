// A simple program that computes the square root of a number
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream> // Read/write files
#include <string>

#define LOG(msg) \
    std::cout << msg << std::endl

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        LOG("hello world");
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }

    // convert input to double
    const double inputValue = atof(argv[1]);

    // calculate square root
    const double outputValue = sqrt(inputValue);
    std::cout << "The square root of " << inputValue << " is " << outputValue
              << std::endl;
    return 0;
}
