#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include <grpcpp/support/server_interceptor.h>

#include "request-models/requests.grpc.pb.h"
#include "request-models/requests.pb.h"


using grpc::Status;
using grpc::experimental::InterceptionHookPoints;
using grpc::experimental::Interceptor;
using grpc::experimental::InterceptorBatchMethods;
using grpc::experimental::ServerInterceptorFactoryInterface;
using grpc::experimental::ServerRpcInfo;

using request_models::Request;


namespace interceptors {

class LoggingInterceptor : public Interceptor {
public:
    void Intercept(InterceptorBatchMethods* methods) override {
        if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::POST_RECV_INITIAL_METADATA)) {
            std::cout << "[Interceptor " << this << "]: POST_RECV_INITIAL_METADATA" << std::endl;

            const auto& metadata = methods->GetRecvInitialMetadata();

            // print all metadata supplied by the client
            for (const auto& [key, value] : *metadata) {
                std::cout << "[Interceptor " << this << "]: " << key << " -> '" << value << "'" << std::endl;
            }

            // searching for auth token
            auto it = metadata->find("authorization");

            // validate auth token
            // TODO: do not forget to split 'Bearer [TOKEN]'
            if (it != metadata->end() && isTokenValid(it->second.data())) {
                std::string token = it->second.data();
                std::cout << "[Interceptor " << this << "]: token='" << token << "'" << std::endl;

                // set user id as metadata from token
                std::string userId = getUserIdFromToken(token);
                metadata->insert({ "x-user-id", userId });
            }
            else {
                std::cout << "[Interceptor " << this << "]: " << "invalid token" << std::endl;
                // this variable indicates whether the received request contained invalid JWT token
                invalidTokenReceived = true;
            }
        }
        else if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::PRE_SEND_STATUS) && invalidTokenReceived) {
            std::cout << "[Interceptor " << this << "]: " << "PRE_SEND_STATUS" << std::endl;
            // once the endpoint returns, we substitute the returned status with UNAUTHENTICATED
            methods->ModifySendStatus(grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid or missing token"));
        }

        methods->Proceed();
    }

private:
    bool isTokenValid(std::string token) const {
        // TODO: apply any nessesary validation of the token
        return true;
    }

    std::string getUserIdFromToken(std::string token) const {
        // TODO: parse JWT token and return the user id stored within the token
        return "1";
    }

private:
    bool invalidTokenReceived = false;
};

class LoggingInterceptorFactory : public ServerInterceptorFactoryInterface {
 public:
  Interceptor* CreateServerInterceptor(ServerRpcInfo* info) override {
    return new LoggingInterceptor();
  }
};

}
