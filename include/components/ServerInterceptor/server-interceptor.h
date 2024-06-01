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
    void Intercept(InterceptorBatchMethods* methods) override;

private:
    bool isTokenValid(std::string token) const;
    std::string getUserIdFromToken(std::string token) const;

private:
    bool invalidTokenReceived = false;
};

class LoggingInterceptorFactory : public ServerInterceptorFactoryInterface {
 public:
  Interceptor* CreateServerInterceptor(ServerRpcInfo* info) override;
};

}
