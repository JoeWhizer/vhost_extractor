#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <boost/filesystem.hpp>
#include "TBBTools.h"
#include "TBBConsole.h"
#include "THosts.h"

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
    std::cout << "-d /path/to/input/dir     - parse all .conf files in this directory\n";
    std::cout << "-r /path/to/vhost.conf    - replace vhost configuration with configuration in apache.conf (only working with -w)\n";
    std::cout << "-w /path/to/apache.conf   - replace vhost from -r option to this apache.conf (only working with -r)\n";
    std::cout << "-v                        - Verbose setting (prints out a result after parsing\n";
    std::cout << "-h                        - Shows this help!\n\n";
}

void printOutput(std::vector<THosts> host_list, bool verbose)
{
    std::cout << host_list.size() << " virtual hosts found!\n\n";
    if (!verbose) return;

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
        printf("%s\n", host_list[i].full_filename.c_str());
        TBBConsole::setTextColor(ConsoleColors::Yellow_TXT);
        printf("Source: ");
        TBBConsole::setTextColor(ConsoleColors::BrightWhite_TXT);
        printf("%s\n\n", host_list[i].input_file.c_str());
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

void verifyArgs(std::string inputFile, std::string outputPath, std::string confDir,
 std::string output_conf="", std::string vhost_to_replace="", bool dirMode = false, bool replaceMode = false)
{
    if (((inputFile == "" && confDir == "") || outputPath == "") || 
        (dirMode && (confDir == "" || outputPath == "")) ||
        (replaceMode && (output_conf == "" || vhost_to_replace == "")))
    {   
        TBBTools::printError("Syntax error", "Please specify input file/path and output directory!");
        usage();
        exit(EXIT_FAILURE);
    }

    if ((inputFile != "") && !boost::filesystem::exists(inputFile))
    {
        TBBTools::printError("File not found", inputFile);
        exit(EXIT_FAILURE);
    }

    if (dirMode && !boost::filesystem::exists(confDir))
    {
        TBBTools::printError("Path not found", confDir);
        exit(EXIT_FAILURE);
    }

    if (replaceMode && !boost::filesystem::exists(vhost_to_replace))
    {
        TBBTools::printError("File not found", vhost_to_replace);
        exit(EXIT_FAILURE);
    }
    else if (replaceMode && !boost::filesystem::exists(output_conf))
    {
        TBBTools::printError("File not found", output_conf);
        exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
        }
    }
}

void parseHosts(std::string inputFile, std::vector<THosts> *host_list, std::string inputPath="")
{
    std::vector<std::string> fileList;

    if (inputPath != "")
    {
        fileList = TBBTools::getFilesFromDirectory(inputPath, ".conf");

        if (fileList.size() == 0) 
        {
            TBBConsole::setTextColor(ConsoleColors::Red_TXT);
            std::cerr << "No '.conf'-files found in path: " << inputPath << std::endl;
            TBBConsole::resetColor();
            exit(EXIT_FAILURE);
        }
    }

    if (inputFile != "" && fileList.size() == 0)
        fileList.push_back(inputFile);

    std::ifstream infile;
    infile.exceptions(std::ifstream::badbit);
    try
    {
        for (std::string file : fileList) 
        {
            std::string server_name = "";
            std::string server_port = "";
            int count = 0;
            int start = 0;
            int end  = 0;
            int found = 0;

            infile.open(file);
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
                    host.input_file = file;
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
            infile.close();
        }       
    }
    catch  (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

std::vector<THosts> writeConfigFiles(std::string outputPath,std::vector<THosts> host_list, std::string vhost_to_extract = "")
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
            infile.open(host_list[i].input_file);
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

void insertVhostAtPosition(THosts host, std::string filename, int position, bool backup = true)
{
    std::ifstream infile;
    std::ifstream orgfile;
    std::ofstream outfile;
    boost::filesystem::path tmppath = boost::filesystem::unique_path();

    try
    {
        boost::filesystem::copy_file(filename, tmppath.c_str());

        infile.open(host.input_file);
        orgfile.open(filename);
        outfile.open(tmppath.native());

        int in_count = 0;
        int out_count = 0;
        for (std::string org_line; getline(orgfile, org_line); )
        {
            in_count++;
            if (in_count < position)
            {
                outfile << org_line << std::endl;
            }
            else if (in_count == position)
            {
                for (std::string line; getline(infile, line); )
                {
                    outfile << line << std::endl;
                }
            }
            else
            {
                outfile << org_line << std::endl;
            }
        }

        if (backup)
        {
            boost::filesystem::copy_file(filename, filename + "_bak");
        }

        const char* f = filename.c_str();
        remove(f);
        rename(tmppath.c_str(), f);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
}