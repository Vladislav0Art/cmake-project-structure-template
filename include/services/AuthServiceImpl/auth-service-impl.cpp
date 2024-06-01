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
    std::cout << "AuthService::Login" << std::endl;

    // if interceptor sets the 'x-user-id' if the token is correct.
    // here retrieve the id if it present, otherwise we return
    auto itr = context->client_metadata().find("x-user-id");

    if (itr == context->client_metadata().end()) {
        return grpc::Status::CANCELLED;
    }

    auto [_, userId] = *itr;

    std::cout << "[Service] user-id=" << userId << "\n";

    // for (auto& [key, value] : context->client_metadata()) {
    //     std::cout << key << " -> '" << value << "'" << std::endl;
    // }

    std::string email = request->email();
    std::string username = "User123";
    std::string password = request->password();

    // std::cout << "email: '" << email << "'\n"
    //             << "username: '" << username << "'\n"
    //             << "password: '" << password << "'\n";

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
