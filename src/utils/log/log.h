//
// Created by dk on 12/13/19.
//

#ifndef _LOG_H_
#define _LOG_H_

#include <glog/logging.h>
#include <string>

void log_init(const char* argv0, int size, int level, std::string dir);

#endif
