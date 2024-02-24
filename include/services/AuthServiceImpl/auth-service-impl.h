#pragma once

#include <iostream>
#include <string>

// grpc
#include "services/auth-service.grpc.pb.h"
#include "services/auth-service.pb.h"

namespace application::services
{

class AuthServiceImpl final : public profile::AuthService::Service {
public:
    ::grpc::Status Login(
        ::grpc::ServerContext* context,
        const ::request_models::LoginRequest* request,
        ::response_models::LoginResponse* response
    ) override;

    ::grpc::Status Register(
        ::grpc::ServerContext* context,
        const ::request_models::RegistrationRequest* request,
        ::response_models::RegistrationResponse* response
    ) override;
};

} // namespace application::services

