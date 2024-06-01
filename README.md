# C++ CMake Project Template

Sample project that demonstrates a possible file structure and CMake configurations that are suitable for small-middle size personal projects. The repository can be used as a starting point for a new project.


## gRPC Interceptors

The project components:

1. `include/components/ClientInterceptor/client-interceptor.hpp`: contains the implementation of the client interceptor; this interceptor is responsible for attaching a JWT token to the request and tokens refreshing if the `StatusCode::UNAUTHORIZED` was returned by the server.
1. `include/components/ServerInterceptor/server-interceptor.hpp`: contains the implementation of the server interceptor; this interceptor is responsible for validating the JWT token. The server interceptor cannot prevent the endpoint accepting a request with an invalid token, therfore the interceptor creates a meta-field named `x-user-id` which indicates whether the token was valid. The endpoint should check the mentioned field and act accordingly: if it present allow access to the resource, otherwise cancel the operation. The interceptor memorizes that token was invalid, thus when the server endpoint returns, it changes the returning `Status` to be 1) `StatusCode::UNAVAILABLE` or 2) `StatusCOde::Unauthorized` depending on whether the should 1) retry the request or 2) should not.
1. `src/proto`: contains the definition of a single service `MyService` with a private endpoint `PrivateResource`.
1. `include/services/MyServiceImpl/my-service-impl.hpp`: contains the implementation of the service `MyService`; the implementation checks whether the `x-user-id` meta-field is present within the metadata coming with the request.
1. `tests/client.cpp`: contains an example of the client code; `SampleClient` class requests the endpoint and retries the request if one failed with `UNAVAILABLE` status code.

Read comments inside the source files to better understand the idea.

**Warning:** the implementation is likely not the way you should implement the gRPC JWT Auth in C++. `AuthMetadataProcessor` is likely to be more suitable for this (See [this](https://stackoverflow.com/questions/41051041/intercept-grpc-c-calls-in-server-and-client)).