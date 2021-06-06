#include "TBBTools.h"
#include <algorithm>
#include <string>

bool TBBTools::findString(std::string input, std::string text)
{
    size_t pos = input.find(text);
    return (pos != std::string::npos);
}

std::string TBBTools::toLower(std::string text)
{
    std::string response = text;
    std::for_each(response.begin(), response.end(), [](char & c)
    {
        c = ::tolower(c);
    });

    return response;
}

std::string toUpper(std::string text)
{
    std::string response = text;
    std::for_each(response.begin(), response.end(), [](char & c)
    {
        c = ::toupper(c);
    });

    return response;
}

bool TBBTools::inRange(unsigned low, unsigned high, unsigned x)
{
    return  ((x-low) <= (high-low));
}

std::string TBBTools::formatLine(int count)
{
    std::string response = "";
    if (inRange(0, 9, count)) {
        response = "Line 000";
    } else if (inRange(10, 99, count)) {
        response = "Line 00";
    } else if (inRange(100, 999, count)) {
        response = "Line 0";
    } else if (inRange(1000, 99999, count)) {
        response = "Line ";
    }
    return response;
}