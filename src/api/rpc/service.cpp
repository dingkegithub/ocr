//
// Created by dk on 12/12/19.
//
#include "service.h"
#include <iostream>
#include "classify_service.h"

using grpc::ServerBuilder;

namespace rpc_server {
    void run_server(string address) {
        ClassifyService service;

        ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << address << std::endl;
        server->Wait();
    }
}
