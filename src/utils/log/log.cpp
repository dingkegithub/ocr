//
// Created by dk on 12/13/19.
//

#include "log.h"

#include <utility>

void log_init(const char* argv0, int size, int level, std::string dir) {
    google::InitGoogleLogging(argv0);
    FLAGS_log_dir = std::move(dir);
    FLAGS_minloglevel = level;
    FLAGS_logtostderr = false;
    FLAGS_max_log_size = size;
}

