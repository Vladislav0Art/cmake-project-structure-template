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

    if (methods->QueryInterceptionHookPoint(
            grpc::experimental::InterceptionHookPoints::POST_RECV_INITIAL_METADATA)) {
        std::cout << "[Interceptor " << this << "]" << std::endl;

        const auto& metadata = methods->GetRecvInitialMetadata();
        if (metadata == nullptr) {
            std::cout << "[Interceptor " << this << "]: " << "metadata is null" << std::endl;
        }
        auto it = metadata->find("authorization");

        if (it != metadata->end() /* && !std::string(it->second.data()).empty() */) {
            std::cout << "[Interceptor " << this << "]: token='" << std::string(it->second.data()) << "'" << std::endl;
            methods->Proceed();
        } else {
            methods->ModifySendStatus(grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid or missing token"));
            // methods->Hijack();
        }
    } else {
        methods->Proceed();
    }
}

Interceptor* LoggingInterceptorFactory::CreateServerInterceptor(ServerRpcInfo* info) {
    return new LoggingInterceptor();
}

}