//
// Created by dk on 12/11/19.
//
#include "option.h"
#include "config.h"
#include "log.h"
#include "server.h"
#include <string>
#include <iostream>
#include "ocr.h"

int main(int argc, char **argv) {
    std::string config_file = parse_cmd_opt(argc, argv);
    cfg::config_t config = cfg::parse(config_file);
    log_init(argv[0], config.log.size, config.log.level, config.log.dir);

    OCR *ocr = OCR::getInstance()->set_template(config.acr.model);

    char address[30] = {0};
    sprintf(address, "0.0.0.0:%d", config.env.listen_port);

    LOG(INFO) << "Server start on: " << address << std::endl;
    http_server::run_server(config.env.listen_port);

    return 0;
}
