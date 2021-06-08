#include "include/TBBTools.h"
#include "include/THosts.h"
#include "include/TMethods.hpp"

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