#include <iostream>
#include <string>

// services
#include "auth-service-impl.h"



namespace application::services
{

::grpc::Status MyServiceImpl::PrivateResource(
    ::grpc::ServerContext* context,
    const ::request_models::Request* request,
    ::response_models::Response* response
) {
    // interceptor sets the 'x-user-id' field into metadata if the token is correct.
    // here retrieve the id if it is present, otherwise we need to stop any further execution
    // (the 'Unauthorized' will be returned by interceptor)
    auto itr = context->client_metadata().find("x-user-id");

    // unauthorized
    if (itr == context->client_metadata().end()) {
        std::cout << "[MyService::PrivateResource]: CANCELLED" << std::endl;
        return grpc::Status::CANCELLED;
    }

    auto [_, userId] = *itr;
    std::cout << "[MyService::PrivateResource]: userId=" << userId << "\n";

    std::string query = request->query();
    std::string userIdStr = std::string(userId.begin(), userId.end());
    std::string username = "User-" + userIdStr;

    response_models::Payload* payload = response->mutable_payload();
    payload->set_user_id(userIdStr);
    payload->set_data(username);

    return grpc::Status::OK;
}


} // namespace application::services
