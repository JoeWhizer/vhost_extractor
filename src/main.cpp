#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <unistd.h>

#include "include/TBBTools.h"

int main(int argc, char* const argv[])
{
    std::string conf_file="";
    std::string target_dir="";
    std::string sname = "";
    int count = 0;
    int start = 0;
    int end  = 0;
    int found = 0;

    while(1)
    {
        int result = getopt(argc, argv, "i:o:");
        if (result == -1) break;
        switch (result)
        {
            case '?': /* unknown parameter */
                break;
            case ':': /* missing argument of a parameter */
                fprintf(stderr, "missing argument.\n");
                break;
            case 'i':
                conf_file = optarg;
                break;
            case 'o':
                target_dir = optarg;
                break;
            default: /* unknown */
                break;
        }
    }

    if (conf_file == "" || target_dir == "")
    {
        std::cerr << "Syntax error! Please specify input file (-i) and output directory (-o)!" << std::endl;
        return EXIT_FAILURE;
    }

    if (!std::filesystem::exists(conf_file))
    {
        std::cout << "File not found: " << conf_file << std::endl;
        return EXIT_FAILURE;
    }

    if (!std::filesystem::exists(target_dir))
    {
        try
        {
            std::filesystem::create_directories(target_dir);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
    }

    std::ifstream infile;
    infile.exceptions(std::ifstream::badbit);
    try
    {
        infile.open(conf_file);
        for (std::string line; getline(infile, line); )
        {
            count++;
            if (TBBTools::findString(TBBTools::toLower(line), "<virtualhost")) 
            {
                start = count;
                if (end > 0)
                {
                    end = 0;
                }
            } 
            else if (TBBTools::findString(TBBTools::toLower(line), "</virtualhost>")) 
            {
                end = count;
            }

            if (start > 0 && end > 0) 
            {
                start = 0;
                end = 0;
                found++;
            }
        }
    }
    catch  (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    if (found > 0)
    {
        std::cout << found << " virtual hosts found!" << std::endl;
    } 
    else 
    {
        std::cout << "No virtual hosts found!" << std::endl;
    }
    
    return EXIT_SUCCESS;
}