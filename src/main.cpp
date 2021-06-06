#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <vector>
#include <variant>
#include "include/TBBTools.h"
#include "include/THosts.h"

std::string extractServerName(std::string line)
{
    std::string delimiter = "=";
    std::string n_line = line;
    size_t pos = 0;
    std::string token = "";
    int count = 0;
    while ((pos = n_line.find(delimiter)) != std::string::npos)
    {
        count++;
        token = n_line.substr(0, pos);
        n_line.erase(0, pos + delimiter.length());
    }
    return n_line;
}

bool verifyArgs(std::string input, std::string output)
{
    if (input == "" || output == "")
    {
        std::cerr << "Syntax error! Please specify input file (-i) and output directory (-o)!" << std::endl;
        return false;
    }

    if (!std::filesystem::exists(input))
    {
        std::cout << "File not found: " << input << std::endl;
        return false;
    }

    if (!std::filesystem::exists(output))
    {
        try
        {
            std::filesystem::create_directories(output);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }
    return true;
}

bool parseHosts(std::string input, std::vector<THosts> *host_list)
{
    std::string server_name = "";
    int count = 0;
    int start = 0;
    int end  = 0;
    int found = 0;

    std::ifstream infile;
    infile.exceptions(std::ifstream::badbit);
    try
    {
        infile.open(input);
        for (std::string line; getline(infile, line); )
        {
            count++;
            std::string n_line = TBBTools::toLower(TBBTools::trim(line, " \t"));
            if (n_line.size() > 0 && TBBTools::checkHashtag(n_line)) continue;            
            if (TBBTools::findString(TBBTools::toLower(n_line), "<virtualhost")) 
            {
                start = count;
                if (end > 0)
                {
                    end = 0;
                }
            } 
            else if (TBBTools::findString(TBBTools::toLower(n_line), "</virtualhost>")) 
            {
                end = count;
            }
            else if (TBBTools::findString(TBBTools::toLower(line), "servername"))
            {
                server_name = extractServerName(n_line);
            }

            if (start > 0 && end > 0) 
            {
                found++;
                THosts host;
                host.start_line = start;
                host.end_line = end;
                host.server_name = server_name;
                host_list->push_back(host);
                start = 0;
                end = 0;
                server_name = "";
            }
        }
    }
    catch  (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

int main(int argc, char* const argv[])
{
    std::string conf_file="";
    std::string target_dir="";
    std::vector<THosts> host_list;

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

    if (!verifyArgs) return EXIT_FAILURE;
    if (!parseHosts(conf_file, &host_list)) return EXIT_FAILURE;

    if (host_list.size() > 0)
    {
        std::cout << host_list.size() << " virtual hosts found!" << std::endl;
        for (int i = 0; i < host_list.size(); i++)
        {
            std::cout << "Host: " << host_list[i].server_name << " -- From line " << host_list[i].start_line << " to line " << host_list[i].end_line << std::endl;
        }
    } 
    else 
    {
        std::cout << "No virtual hosts found!" << std::endl;
    }

    return EXIT_SUCCESS;
}