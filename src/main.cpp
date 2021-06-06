#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <vector>
#include "include/TBBTools.h"
#include "include/THosts.h"

std::string extractServerName(std::string line)
{
    std::string delimiter = "=";
    size_t pos = 0;
    std::string token = "";
    int count = 0;
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
        count++;
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
    }
    return line;
}

std::string extractServerPort(std::string line)
{
    std::string delimiter = ":";
    size_t pos = 0;
    std::string token = "";
    int count = 0;
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
        count++;
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
    }
    // remove last character '>'
    line = line.substr(0, line.size() -1);
    return line;
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
            std::cout << "Path not found: " << output << " - creating directory...";
            std::filesystem::create_directories(output);
            std::cout << "done" << std::endl;
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
    std::string server_port = "";
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
            if (TBBTools::findString(n_line, "<virtualhost"))
            {
                start = count;
                if (end > 0)
                {
                    end = 0;
                }

                if (TBBTools::findString(n_line, ":"))
                {
                    server_port = extractServerPort(n_line);
                }
            } 
            else if (TBBTools::findString(n_line, "</virtualhost>")) 
            {
                end = count;
            }
            else if (TBBTools::findString(n_line, "servername"))
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
                host.server_port = server_port;
                host_list->push_back(host);
                start = 0;
                end = 0;
                server_name = "";
                server_port = "";
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

std::vector<THosts> writeConfigFiles(std::string inputFile, std::string outputPath,std::vector<THosts> host_list)
{
    std::ifstream infile;
    std::ofstream outfile;

    for (int i = 0; i < host_list.size(); i++)
    {
        std::string filename = "";
        if (host_list[i].server_port != "")
            filename = host_list[i].server_name + "_" + host_list[i].server_port + ".conf";
        else
            filename = host_list[i].server_name + ".conf";
        
        host_list[i].full_filename = outputPath;
        host_list[i].full_filename /= filename;
        std::cout << host_list[i].full_filename << std::endl;
        try
        {
            infile.open(inputFile);
            outfile.open(host_list[i].full_filename);
            int count = 0;
            for (std::string line; getline(infile, line); )
            {
                if (TBBTools::inRange(host_list[i].start_line, host_list[i].end_line, ++count))
                    outfile << line << std::endl;
            }
            infile.close();
            outfile.close();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    return host_list;
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

    if (!verifyArgs(conf_file, target_dir)) return EXIT_FAILURE;

    if (!parseHosts(conf_file, &host_list)) return EXIT_FAILURE;

    host_list = writeConfigFiles(conf_file, target_dir, host_list);

    if (host_list.size() > 0)
    {
        std::cout << host_list.size() << " virtual hosts found!" << std::endl;
        for (int i = 0; i < host_list.size(); i++)
        {
            std::cout << "Host: " << host_list[i].server_name << " - " << 
                         "Port: " << host_list[i].server_port <<
                         " -- From line " << host_list[i].start_line << 
                         " to line " << host_list[i].end_line << " - " <<
                         "Filename: " << host_list[i].full_filename << std::endl;
        }
    } 
    else 
    {
        std::cout << "No virtual hosts found!" << std::endl;
    }

    return EXIT_SUCCESS;
}