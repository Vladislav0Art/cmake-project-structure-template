#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <optional>

#include <grpcpp/grpcpp.h>

#include "doctest.h"

// grpc
#include "services/my-service.grpc.pb.h"
#include "services/my-service.pb.h"

#include "components/ClientInterceptor/client-interceptor.h"



namespace doctest {

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using request_models::Request;
using response_models::Response;
using request_models::Request;
using response_models::Response;
using profile::MyService;

using namespace interceptors;

template<typename Payload>
using Result = std::pair<std::optional<Payload>, Status>;


class SampleClient {
public:
    SampleClient(std::shared_ptr<Channel> channel)
        : m_stub(MyService::NewStub(channel)) {}

    Result<response_models::Payload> AccessPrivateResource(const std::string& query) {
        Request request;
        request.set_query(query);

        Response reply;
        ClientContext context;

        Status status = m_stub->PrivateResource(&context, request, &reply);

        if (status.ok()) {
            return {reply.payload(), status};
        }
        if (status.error_code() == grpc::StatusCode::UNAVAILABLE) {
            // TODO: client interceptor should have refreshed the JWT token via calling the refresh method of AuthService,
            // now we try to request the service more time.
            return AccessPrivateResource(query);
        }
        else {
            // any other error including UNAUTHORIZED should result in a request failure
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return {std::nullopt, status};
        }
    }

private:
    std::unique_ptr<MyService::Stub> m_stub;
};


TEST_CASE("Failed authrization scenario") {
    auto channel = grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials());
    SampleClient client(channel);

    std::string query = "some query to the private resource";
    auto [payload, status] = client.AccessPrivateResource(query);

    if (payload.has_value()) {
        std::cout << "user_id=" << payload.value().user_id() << ", data=" << payload.value().data() << "\n";
    }
    else {
        std::cout << "Request failed with status: '" << status.error_message() << "'\n";
    }
}


TEST_CASE("Successful authrization scenario") {
    std::string token = "1234";

    std::vector<std::unique_ptr<grpc::experimental::ClientInterceptorFactoryInterface>> interceptor_creators;
    interceptor_creators.push_back(std::make_unique<ClientInterceptorFactory>(token));

    auto channel = grpc::experimental::CreateCustomChannelWithInterceptors(
        "0.0.0.0:50051", grpc::InsecureChannelCredentials(), grpc::ChannelArguments(), std::move(interceptor_creators));

    SampleClient client(channel);

    std::string query = "some query to the private resource";
    auto [payload, status] = client.AccessPrivateResource(query);

    if (payload.has_value()) {
        std::cout << "user_id=" << payload.value().user_id() << ", data=" << payload.value().data() << "\n";
    }
    else {
        std::cout << "Request failed with status: '" << status.error_message() << "'\n";
    }
}


}