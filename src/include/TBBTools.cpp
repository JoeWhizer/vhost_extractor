#include "TBBTools.h"
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>

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

int TBBTools::countDigit(int number) {
   int count = 0;
   while(number != 0) {
      number = number / 10;
      count++;
   }
   return count;
}

int TBBTools::getLineNumbers(std::string filename)
{
    int count = 0;
    std::ifstream infile;
    infile.exceptions(std::ifstream::badbit);
    try
    {
        infile.open(filename);
        for (std::string line; getline(infile, line); )
        {
            count++;
        }
    }
    catch  (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }    

    return count;
}

std::string TBBTools::formatLine(int line, int maxlines, std::string prefix="Line")
{
    std::string separator = "";
    int diff = maxlines - line;

    for (int i = 0; i < diff; i++)
    {
        separator += "0";
    }

    return prefix + " " + separator + std::to_string(line) + ": ";
}