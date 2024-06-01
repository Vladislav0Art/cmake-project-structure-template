#include <iostream>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "doctest.h"

// grpc
#include "services/auth-service.grpc.pb.h"
#include "services/auth-service.pb.h"

#include "components/ClientInterceptor/client-interceptor.h"



namespace doctest {

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using request_models::LoginRequest;
using response_models::LoginResponse;
using request_models::RegistrationRequest;
using response_models::RegistrationResponse;
using profile::AuthService;

using namespace interceptors;



class AuthServiceClient {
public:
    AuthServiceClient(std::shared_ptr<Channel> channel)
        : m_stub(AuthService::NewStub(channel)) {}

    std::string Login(const std::string& email, const std::string& password) {
        LoginRequest request;
        request.set_email(email);
        request.set_password(password);

        LoginResponse reply;
        ClientContext context;

        Status status = m_stub->Login(&context, request, &reply);

        if (status.ok()) {
            return "Login successful: " + reply.payload().email() + ", " + reply.payload().username();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "Login failed";
        }
    }

    std::string Register(const std::string& email, const std::string& username, const std::string& password) {
        RegistrationRequest request;
        request.set_email(email);
        request.set_username(username);
        request.set_password(password);

        RegistrationResponse reply;
        ClientContext context;

        Status status = m_stub->Register(&context, request, &reply);

        if (status.ok()) {
            return "Registration successful: " + reply.payload().email() + ", " + reply.payload().username();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "Registration failed";
        }
    }

private:
    std::unique_ptr<AuthService::Stub> m_stub;
};



TEST_CASE("Connect to the gRPC server") {

    std::string email = "test@example.com";
    std::string username = "testuser";
    std::string password = "12345";

    // failed auth
    {
        auto channel = grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials());
        AuthServiceClient client(channel);

        std::string login_response = client.Login(email, password);
        std::cout << "Failed Login response: " << login_response << std::endl;
    }


    // successful auth
    {
        std::string token = "1234";

        std::vector<std::unique_ptr<grpc::experimental::ClientInterceptorFactoryInterface>> interceptor_creators;
        interceptor_creators.push_back(std::make_unique<ClientInterceptorFactory>(token));

        auto channel = grpc::experimental::CreateCustomChannelWithInterceptors(
            "0.0.0.0:50051", grpc::InsecureChannelCredentials(), grpc::ChannelArguments(), std::move(interceptor_creators));

        AuthServiceClient client(channel);

        std::string login_response = client.Login(email, password);
        std::cout << "Successful Login response: " << login_response << std::endl;
    }

    // std::string register_response = client.Register(email, username, password);
    // std::cout << "Register response: " << register_response << std::endl;
}


TEST_CASE("Test interceptors") {

}


}