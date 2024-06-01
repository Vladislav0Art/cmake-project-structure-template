#include "server-interceptor.h"


namespace interceptors {

void LoggingInterceptor::Intercept(InterceptorBatchMethods* methods) {
    // std::cout << "Entering interceptor " << (this) << std::endl;
    /*if (methods->QueryInterceptionHookPoint(
            InterceptionHookPoints::POST_RECV_MESSAGE)) {
        LoginRequest* request = static_cast<LoginRequest*>(methods->GetRecvMessage());
        std::cout << "[Interceptor " << this << "]: "
                  << "email='" << request->email() << "', password='" << request->password() << "'" << std::endl;
    }
    methods->Proceed();*/

    if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::POST_RECV_INITIAL_METADATA)) {
        std::cout << "[Interceptor " << this << "]: POST_RECV_INITIAL_METADATA" << std::endl;

        const auto& metadata = methods->GetRecvInitialMetadata();
        auto it = metadata->find("authorization");

        for (const auto& [key, value] : *metadata) {
            std::cout << key << " -> " << value << std::endl;
        }

        // TODO: check token presence and validity
        if (it != metadata->end() && !std::string(it->second.data()).empty()) {
            std::cout << "[Interceptor " << this << "]: token='" << std::string(it->second.data()) << "'" << std::endl;
            // methods->Proceed();
        }
        else {
            std::cout << "[Interceptor " << this << "]: " << "Auth token not found" << std::endl;
            invalidTokenReceived = true;
            // methods->Proceed();
        }
    }
    else if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::PRE_SEND_STATUS) && invalidTokenReceived) {
        std::cout << "[Interceptor " << this << "]: " << "PRE_SEND_STATUS" << std::endl;
        methods->ModifySendStatus(grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid or missing token"));
    }

    methods->Proceed();
}

Interceptor* LoggingInterceptorFactory::CreateServerInterceptor(ServerRpcInfo* info) {
    return new LoggingInterceptor();
}

}