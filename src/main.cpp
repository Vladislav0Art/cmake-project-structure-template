#include <iostream>


#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>


#include "profile-service.grpc.pb.h"


#include "components/HelloWorldComponent/hello-world-component.h"
#include "mylib/mylib.hpp"




using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using namespace profile;


class ProfileServiceImpl final : public ProfileService::Service {
public:
    Status ChangeProfile(ServerContext *context, const ProfileRequest *request, ProfileReply *reply) override {
        return Status::OK;
    }
};




int main() {
    HelloWorldComponent component;

    component.greet("World");
    component.greet("C++");
    component.greet("CMake");

    mylib::MyLib lib;
    std::cout << lib.sum(1, 3) << "\n";

  std::string server_address("0.0.0.0:50051");

  grpc::EnableDefaultHealthCheckService(true);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  ProfileServiceImpl service;
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

    return 0;
}