#pragma once
#include <string>

class THosts
{
    public:
        int start_line = 0;
        int end_line = 0;
        std::string input_file = "";
        std::string server_name = "";
        std::string server_port = "";
        boost::filesystem::path full_filename = "";
};