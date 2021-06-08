#pragma once

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

class TBBTools 
{
    public:
        static bool findString(std::string input, std::string text);
        static bool inRange(unsigned low, unsigned high, unsigned x);
        static int countDigit(int number);
        static std::string toLower(std::string text);
        static std::string toUpper(std::string text);
        static int getLineNumbers(std::string filename);
        static std::string formatLine(int line, int maxlines, std::string prefix);
        static std::string trim(const std::string& str, const std::string& whitespace);
        static std::string reduce(const std::string& str,const std::string& fill,const std::string& whitespace);
        static bool checkHashtag(std::string line);
        static std::vector<std::string> getFilesFromDirectory(boost::filesystem::path path, std::string ext);
        static void removeLinesFromFile(std::string filename, int start, int end, bool backup);
};