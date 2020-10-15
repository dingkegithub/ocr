//
// Created by dk on 12/12/19.
//

#ifndef _CLASSIFY_SERVICE_H_
#define _CLASSIFY_SERVICE_H_

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "acr.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::ServerContext;
using grpc::Status;

using lacr::Classify;
using lacr::Request;
using lacr::Response;

namespace rpc_server {

    class ClassifyService final : public Classify::Service {
    public:
        explicit ClassifyService() {
        }

    public:
        Status Recognise(ServerContext *context, const Request *request, Response *response);
    };
}
#endif
