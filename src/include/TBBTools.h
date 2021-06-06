#pragma once

#include <string>

class TBBTools 
{
    public:
        static bool findString(std::string input, std::string text);
        static bool inRange(unsigned low, unsigned high, unsigned x);
        static std::string toLower(std::string text);
        static std::string toUpper(std::string text);
        static std::string formatLine(int count);
};