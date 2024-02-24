#include <iostream>
#include <string>

// grpc
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

// services
#include "services/AuthServiceImpl/auth-service-impl.h"

// components
#include "components/HelloWorldComponent/hello-world-component.h"

// libs
#include "mylib/mylib.hpp"




int main() {
    using grpc::Server;
    using grpc::ServerBuilder;
    using grpc::ServerContext;
    using grpc::Status;

    application::services::AuthServiceImpl service;

    std::string server_address("0.0.0.0:50051");
    grpc::EnableDefaultHealthCheckService(true);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // registering services
    builder.RegisterService(&service);

    // running server
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

    return 0;
}