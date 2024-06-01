#include <iostream>
#include <string>

// services
#include "auth-service-impl.h"



namespace application::services
{

::grpc::Status AuthServiceImpl::Login(
    ::grpc::ServerContext* context,
    const ::request_models::LoginRequest* request,
    ::response_models::LoginResponse* response
) {
    std::string email = request->email();
    std::string username = "User123";
    std::string password = request->password();

    std::cout << "email: '" << email << "'\n"
                << "username: '" << username << "'\n"
                << "password: '" << password << "'\n";

    if (password.size() < 4) {
        // Return error status
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Password must contain at least 4 symbols");
    }

    // If password is valid, set payload and return OK status
    response_models::Payload* payload = response->mutable_payload();
    payload->set_email(email);
    payload->set_username(username);

    return grpc::Status::OK;
}

::grpc::Status AuthServiceImpl::Register(
    ::grpc::ServerContext* context,
    const ::request_models::RegistrationRequest* request,
    ::response_models::RegistrationResponse* response
) {
    return grpc::Status::OK;
}


} // namespace application::services
