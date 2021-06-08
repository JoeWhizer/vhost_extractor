#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <boost/filesystem.hpp>
#include "include/TBBTools.h"
#include "include/TBBConsole.h"
#include "include/THosts.h"

void usage()
{
    TBBConsole::setTextColor(ConsoleColors::Yellow_TXT);
    std::cout << "vhost_extractor v1.1\n";
    TBBConsole::resetColor();
    std::cout << "Usage: vhost_extractor -i /path/to/apache.conf -o /path/to/output/dir\n\n";
    std::cout << "Options:\n";
    std::cout << "-i /path/to/apache.conf   - Full path to the configuration that you want to parse\n";
    std::cout << "-o /path/to/output/dir    - Path where extracted vhost will be stored (if not exists, it will be created)\n";
    std::cout << "-s servername             - Extract single configuration by ServerName\n";
    std::cout << "-v                        - Verbose setting (prints out a result after parsing\n";
    std::cout << "-h                        - Shows this help!\n\n";
}

void printOutput(std::vector<THosts> host_list)
{
    std::cout << host_list.size() << " virtual hosts found!\n\n";
    std::cout << "The following hosts were extracted:\n";
    for (int i = 0; i < host_list.size(); i++)
    {
        if (host_list[i].full_filename == "")
            continue;

        if ( host_list[i].server_port == "" ) host_list[i].server_port = "N/A";

        TBBConsole::setTextColor(ConsoleColors::Green_TXT);
        printf("Server Nr. %d\n", i +1);
        TBBConsole::setTextColor(ConsoleColors::Yellow_TXT);
        printf("ServerName: ");
        TBBConsole::setTextColor(ConsoleColors::BrightWhite_TXT);
        printf("%s\n", host_list[i].server_name.c_str());
        TBBConsole::setTextColor(ConsoleColors::Yellow_TXT);
        printf("Port: ");
        TBBConsole::setTextColor(ConsoleColors::BrightWhite_TXT);
        printf("%s\n", host_list[i].server_port.c_str());
        TBBConsole::setTextColor(ConsoleColors::Yellow_TXT);
        printf("From line ");
        TBBConsole::setTextColor(ConsoleColors::BrightWhite_TXT);
        printf("%d ", host_list[i].start_line);
        TBBConsole::setTextColor(ConsoleColors::Yellow_TXT);
        printf("to line ");
        TBBConsole::setTextColor(ConsoleColors::BrightWhite_TXT);
        printf("%d \n", host_list[i].end_line);
        TBBConsole::setTextColor(ConsoleColors::Yellow_TXT);
        printf("Filename: ");
        TBBConsole::setTextColor(ConsoleColors::BrightWhite_TXT);
        printf("%s\n\n", host_list[i].full_filename.c_str());
    }
}

std::string extractLastEntrybyDelimiter(std::string line, std::string delimiter, bool removeLastChar = false)
{
    size_t pos = 0;
    std::string token = "";
    int count = 0;
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
        count++;
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
    }
    if (removeLastChar)
        line = line.substr(0, line.size() -1);

    return line;
}

std::string extractFirstEntrybyDelimiter(std::string line, std::string delimiter)
{
    size_t pos = 0;
    std::string token = "";
    std::string n_line = line;
    while ((pos = n_line.find(delimiter)) != std::string::npos)
    {
        token = n_line.substr(0, pos);
        return token;
    }
    return line;
}

bool verifyArgs(std::string inputFile, std::string outputPath)
{
    if (inputFile == "" || outputPath == "")
    {
        TBBConsole::setTextColor(Red_TXT);
        std::cerr << "Syntax error! Please specify input file (-i) and output directory (-o)!\n\n";
        TBBConsole::resetColor();
        usage();
        return false;
    }

    if (!boost::filesystem::exists(inputFile))
    {
        TBBConsole::setTextColor(Red_TXT);
        std::cerr << "File not found: " << inputFile << std::endl;
        return false;
        TBBConsole::resetColor();
    }

    if (!boost::filesystem::exists(outputPath))
    {
        try
        {
            std::cout << "Path not found: " << outputPath << " - creating directory...";
            boost::filesystem::create_directories(outputPath);
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

bool parseHosts(std::string inputFile, std::vector<THosts> *host_list)
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
        infile.open(inputFile);
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
                    server_port = extractLastEntrybyDelimiter(n_line, ":", true);
                }
            } 
            else if (TBBTools::findString(n_line, "</virtualhost>")) 
            {
                end = count;
            }
            else if (TBBTools::findString(n_line, "servername"))
            {
                server_name = extractLastEntrybyDelimiter(n_line, " ");
                if (TBBTools::findString(server_name, ":"))
                    server_name = extractFirstEntrybyDelimiter(server_name, ":");
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

std::vector<THosts> writeConfigFiles(std::string inputFile, std::string outputPath,std::vector<THosts> host_list, std::string vhost_to_extract = "")
{
    std::ifstream infile;
    std::ofstream outfile;

    for (int i = 0; i < host_list.size(); i++)
    {
        if ((vhost_to_extract != "") && (TBBTools::toLower(host_list[i].server_name) != TBBTools::toLower(vhost_to_extract)))
            continue;

        std::string filename = "";
        if (host_list[i].server_port != "")
            filename = host_list[i].server_name + "_" + host_list[i].server_port + ".conf";
        else
            filename = host_list[i].server_name + ".conf";
        
        host_list[i].full_filename = outputPath;
        host_list[i].full_filename /= filename;
        try
        {
            infile.open(inputFile);
            outfile.open(host_list[i].full_filename.string());
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
    std::string vhost_to_extract="";
    bool verbose = false;

    while(1)
    {
        int result = getopt(argc, argv, "i:o:s:vh");
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
            case 's':
                vhost_to_extract = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                usage();
                return EXIT_SUCCESS;
            default: /* unknown */
                break;
        }
    }

    if (!verifyArgs(conf_file, target_dir)) return EXIT_FAILURE;

    if (!parseHosts(conf_file, &host_list)) return EXIT_FAILURE;

    host_list = writeConfigFiles(conf_file, target_dir, host_list, vhost_to_extract);

    if (!verbose) return EXIT_SUCCESS;

    if (host_list.size() > 0)
    {
        printOutput(host_list);
    } 
    else 
    {
        std::cout << "No virtual hosts found!" << std::endl;
    }
    return EXIT_SUCCESS;
}