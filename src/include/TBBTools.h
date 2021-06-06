#pragma once

#include <string>
#include <fstream>

class TBBTools 
{
    public:
        static bool findString(std::string input, std::string text);
        static bool inRange(unsigned low, unsigned high, unsigned x);
        static int countDigit(int number);
        static std::string toLower(std::string text);
        static std::string toUpper(std::string text);
        static int getLineNumbers(std::string filename);
        static std::string formatLine(int line, int maxlines);
};