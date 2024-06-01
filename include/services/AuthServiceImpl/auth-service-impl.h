#pragma once

#include <iostream>
#include <string>

// grpc
#include "services/my-service.grpc.pb.h"
#include "services/my-service.pb.h"

namespace application::services
{

class MyServiceImpl final : public profile::MyService::Service {
public:
    ::grpc::Status PrivateResource(
        ::grpc::ServerContext* context,
        const ::request_models::Request* request,
        ::response_models::Response* response
    ) override;
};

} // namespace application::services

