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

        // print all metadata supplied by the client
        for (const auto& [key, value] : *metadata) {
            std::cout << key << " -> '" << value << "'" << std::endl;
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
            std::cout << "[Interceptor " << this << "]: " << "token not found" << std::endl;
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


bool LoggingInterceptor::isTokenValid(std::string token) const {
    // TODO: apply any nessesary validation of the token
    return true;
}

std::string LoggingInterceptor::getUserIdFromToken(std::string token) const {
    // TODO: parse JWT token and
    return "1";
}

Interceptor* LoggingInterceptorFactory::CreateServerInterceptor(ServerRpcInfo* info) {
    return new LoggingInterceptor();
}

}