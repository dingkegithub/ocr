//
// Created by dk on 12/13/19.
//
#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>

using std::string;

namespace cfg {

    typedef struct env_s {
        int process_num;
        int listen_port;
    } env_t;

    typedef struct log_s {
        int level;
        int size;
        string dir;
        string name;
    } log_t;

    typedef struct acr_s {
        string model;
        string mean;
        string deploy;
        string word;
        bool gpu_enable;
        int gpu_number;
    } acr_t;

    typedef struct config_s {
        env_t env;
        log_t log;
        acr_t acr;
    } config_t;

    config_t parse(const string &file);
}

#endif
