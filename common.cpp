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