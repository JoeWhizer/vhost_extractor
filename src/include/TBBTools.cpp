#include "TBBTools.h"
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>


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
    int diff = countDigit(maxlines) - countDigit(line);

    for (int i = 0; i < diff; i++)
    {
        separator += "0";
    }

    return prefix + " " + separator + std::to_string(line) + ": ";
}

std::string TBBTools::trim(const std::string& str, const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string TBBTools::reduce(const std::string& str, const std::string& fill = " ", const std::string& whitespace = " \t")
{
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

bool TBBTools::checkHashtag(std::string line)
{
    if (line.at(0) == '#') 
    {
        return true;
    }
    return false;
}

std::vector<std::string> TBBTools::getFilesFromDirectory(boost::filesystem::path path, std::string ext)
{
    using namespace boost::filesystem;
    std::vector<std::string> fileList;

    directory_iterator end_itr;
    for (directory_iterator itr(path); itr != end_itr; itr++)
    {
        if (is_regular_file(itr->path()))
        {
            std::string currentFile = itr->path().string();
            if (toLower(ext) == toLower(extension(currentFile)))
            {
                fileList.push_back(currentFile);
            }
        }
    }
    return fileList;
}

void TBBTools::removeLinesFromFile(std::string filename, int start, int end, bool backup = true)
{
    std::ifstream infile;
    std::ofstream outfile;
    boost::filesystem::path tmppath = boost::filesystem::unique_path();
    
    infile.open(filename);
    outfile.open(tmppath.native());

    int count = 0;
    for (std::string line; getline(infile, line); )
    {
        if (!TBBTools::inRange(start, end, ++count))
            outfile << line << std::endl;
    }

    if (backup)
    {
        boost::filesystem::copy_file(filename, filename + "_bak");
    }

    const char* f = filename.c_str();
    remove(f);
    rename(tmppath.c_str(), f);
}