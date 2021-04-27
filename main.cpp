#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream> // Read/write files
#include <string>
#include <config.h>

#define LOG(msg) \
    std::cout << msg << std::endl

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        LOG("hello world");
        std::cout << argv[0] << " Version " << IMGTOGCODE_VERSION_MAJOR << "." << IMGTOGCODE_VERSION_MINOR << '\n';
        return 1;
    }

    return 0;
}
