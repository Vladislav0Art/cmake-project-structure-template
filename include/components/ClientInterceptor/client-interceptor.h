#pragma once

#include <grpcpp/grpcpp.h>



namespace interceptors
{

class ClientInterceptor : public grpc::experimental::Interceptor {
public:
    ClientInterceptor(const std::string& token) : token_(token) {}

    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
        if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::PRE_SEND_INITIAL_METADATA)) {
            auto* metadata = methods->GetSendInitialMetadata();
            metadata->insert({ "authorization", "Bearer " + token_ });
        }
        methods->Proceed();
    }

private:
    std::string token_;
};

class ClientInterceptorFactory : public grpc::experimental::ClientInterceptorFactoryInterface {
public:
    ClientInterceptorFactory(const std::string& token) : token_(token) {}

    grpc::experimental::Interceptor* CreateClientInterceptor(grpc::experimental::ClientRpcInfo* info) override {
        return new ClientInterceptor(token_);
    }

private:
    std::string token_;
};

} // namespace interceptors
