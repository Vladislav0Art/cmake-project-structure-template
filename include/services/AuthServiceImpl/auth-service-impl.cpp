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
    std::string username = "Username taken from the database";
    std::string password = request->password();

    std::cout << "email: " << email << "\n"
                << "username: " << username << "\n"
                << "password: " << password << "\n";

    if (password.size() < 4) {
        // Set error details
        common::ErrorDetails error_details;
        error_details.set_has_error(true);
        error_details.add_details("Password must be at least 4 characters long.");

        // Set the error details in the response
        response->mutable_details()->CopyFrom(error_details);

        // Return error status
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid password");
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
