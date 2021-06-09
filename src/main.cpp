#include "include/TBBTools.h"
#include "include/THosts.h"
#include "include/TMethods.hpp"

int main(int argc, char* const argv[])
{
    std::string conf_file="";
    std::string target_dir="";
    std::vector<THosts> host_list;
    std::string vhost_to_extract="";
    std::string conf_dir="";
    std::string vhost_to_replace="";
    std::string output_conf="";
    bool backup = true;
    bool verbose = false;
    bool replaceMode = false;
    bool directoryMode = false;

    while(1)
    {
        int result = getopt(argc, argv, "i:o:s:d:r:w:vh");
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
            case 'd':
                conf_dir = optarg;
                directoryMode = true;
                break;
            case 'r':
                vhost_to_replace = optarg;
                replaceMode = true;
                break;
            case 'w':
                output_conf = optarg;
                replaceMode = true;
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

    verifyArgs(conf_file, target_dir, conf_dir, output_conf, vhost_to_replace, directoryMode, replaceMode);
    
    if (!replaceMode) {
        parseHosts(conf_file, &host_list, conf_dir);
        host_list = writeConfigFiles(target_dir, host_list, vhost_to_extract);
    }
    else
    {
        std::vector<THosts> n_host_list;

        parseHosts(vhost_to_replace, &host_list);
        parseHosts(output_conf, &n_host_list);
        int start = 0;
        for (int i = 0; i < n_host_list.size(); i++)
        {
            if ((n_host_list[i].server_name == host_list[0].server_name) && (n_host_list[i].server_port == host_list[0].server_port))
            {
                start = n_host_list[i].start_line;
                break;
            }
        }
        insertVhostAtPosition(host_list[0], output_conf, start, backup);
    }

    if (host_list.size() > 0)
        printOutput(host_list, verbose);
    else 
        std::cout << "No virtual hosts found!" << std::endl;

    return EXIT_SUCCESS;
}