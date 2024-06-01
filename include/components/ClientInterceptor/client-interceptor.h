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
        else if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::PRE_RECV_STATUS)) {
            grpc::Status* status = methods->GetRecvStatus();
            if (status->error_code() == grpc::StatusCode::UNAUTHENTICATED) {
                // TODO: update the token here
                // assume that the client will try to access the resource one more time,
                // with the updated token
                token_ = "1234 [updated]";
                bool tokenUpdateSucceeded = shouldTokenRefreshingSucceed;

                shouldTokenRefreshingSucceed = false;

                // if the token update was successful then set the status to be the following (see: https://grpc.github.io/grpc/core/md_doc_statuscodes.html),
                // it means that the client should try to access the resource again
                if (tokenUpdateSucceeded) {
                    *status = grpc::Status(grpc::StatusCode::UNAVAILABLE, "JWT token refreshed. Try again.");
                }
            }
        }

        methods->Proceed();
    }

private:
    std::string token_;
    bool shouldTokenRefreshingSucceed = true;
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
