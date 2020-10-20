//
// Created by dk on 12/14/19.
//

#ifndef _SERVER_H_
#define _SERVER_H_

#include "classify.h"
#include "crow_all.h"
#include "middleware.h"

namespace http_server {
    void run_server(int port) {
        crow::App<OcrMiddleWare> app;

        router(app);
        app.port(port).multithreaded().run();
    }
}

#endif
