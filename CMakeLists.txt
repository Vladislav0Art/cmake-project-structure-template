cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProject CXX)

#### adding subdirs
add_subdirectory(./include/)

#### variables propagations
# COMPONENTS_SRCS       - propagated from ./include/components
# UTILS_SRCS            - propagated from ./include/utils


#### setting global props
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If gRPC locally installed, add into cmake: -DCMAKE_PREFIX_PATH=path/to/installation (mine is: /Users/Vladislav.Artiukhov/.local)
find_package(Protobuf REQUIRED CONFIG)
# find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED CONFIG)


message(NOTICE "Binary dir: " ${CMAKE_CURRENT_BINARY_DIR})
message(NOTICE "Components srcs: " ${COMPONENTS_SRCS})
message(NOTICE "Utils srcs: " ${UTILS_SRCS})

message(NOTICE "Protobuf: " ${Protobuf_INCLUDE_DIR})


add_executable(main
    src/main.cpp
    ${COMPONENTS_SRCS}
    ${UTILS_SRCS}
)


target_include_directories(main PUBLIC
    ./include
    ./libs
    ${CMAKE_CURRENT_BINARY_DIR}

    ${OPENSSL_CRYPTO_LIBRARY}
    ${OPENSSL_INCLUDE_DIR}
    ${Protobuf_INCLUDE_DIRS}

    ./build/service.grpc.pb.cc
    ./build/service.pb.cc
)

# Generate gRPC services inside ./buide folder via Protobuf and gRPC plugin:
# /Users/Vladislav.Artiukhov/.local/bin/protoc --grpc_out=./build --cpp_out=./build --plugin=protoc-gen-grpc=/Users/Vladislav.Artiukhov/.local/bin/grpc_cpp_plugin service.proto

target_link_libraries(main
    Threads::Threads
    gRPC::grpc++
    # protobuf::libprotobuf
    # ${Protobuf_LIBRARIES}
)