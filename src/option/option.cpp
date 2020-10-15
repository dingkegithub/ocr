//
// Created by dk on 12/13/19.
//

#include <string>
#include <iostream>
#include <getopt.h>
#include "option.h"


void usage() {
    std::cout << "Options: " << std::endl;
    std::cout << "-h --help   retrieve help" << std::endl;
    std::cout << "-c --conf   required: config file" << std::endl;
}

string parse_cmd_opt(int argc, char **pString) {

    int ret;
    string config;

    while (true) {
        int opt_idx = 0;
        static struct option opts[] = {
                {"help", no_argument, NULL, 'h'},
                {"conf", required_argument, NULL, 'c'},
        };

        ret = getopt_long(argc, pString, "c:h", opts, &opt_idx);
        if (ret == -1) {
            usage();
            break;
        }

        switch (ret) {
            case 'c': {
                config = std::string(optarg);
                break;
            }

            case 'h': {
                usage();
                exit(-1);
            }

            default:
                usage();
                exit(-1);
        }
    }

    return config;
}
