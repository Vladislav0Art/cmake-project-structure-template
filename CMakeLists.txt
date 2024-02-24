cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProject CXX)

#### setting global props
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#### adding subdirs
add_subdirectory(./include/)

#### variables propagations
# COMPONENTS_SRCS       - propagated from ./include/components
# SERVICES_SRCS         - propagated from ./include/services
message(NOTICE "Components srcs: " ${COMPONENTS_SRCS})
message(NOTICE "Services srcs: " ${SERVICES_SRCS})


message(NOTICE "Binary dir: " ${CMAKE_CURRENT_BINARY_DIR})
message(NOTICE "Source dir: " ${CMAKE_SOURCE_DIR})


# If gRPC is locally installed, add into cmake: `-DCMAKE_PREFIX_PATH=path/to/installation` (mine is: /Users/Vladislav.Artiukhov/.local)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)


# Paths must match the destination path set inside generate-proto.sh:
# Contains all the files generated inside build folder ending with pb.cc (i.e. grpc and proto cc-files)
file(GLOB proto_srcs "${CMAKE_SOURCE_DIR}/build/**/*.pb.cc")
# Contains all the files generated inside build folder ending with pb.h (i.e. grpc and proto h-files)
file(GLOB proto_hdrs "${CMAKE_SOURCE_DIR}/build/**/*.pb.h")

message(NOTICE "proto_srcs: " ${proto_srcs})
message(NOTICE "proto_hdrs: " ${proto_hdrs})

# Creating library with the grpc components
add_library(grpc_lib
    ${proto_srcs}
    ${proto_hdrs}
)

target_link_libraries(grpc_lib
    gRPC::grpc++
    protobuf::libprotobuf
)

target_include_directories(grpc_lib PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)


# Creating executable
add_executable(main
    src/main.cpp

    ${COMPONENTS_SRCS}
    ${SERVICES_SRCS}
)

target_link_libraries(main
    grpc_lib # linking the created library

    absl::flags_parse
    gRPC::grpc++
    protobuf::libprotobuf
)

target_include_directories(main PUBLIC
    ./include
    ./libs
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Generate gRPC services inside ./buide folder via Protobuf and gRPC plugin:
# /Users/Vladislav.Artiukhov/.local/bin/protoc --grpc_out=./build --cpp_out=./build --plugin=protoc-gen-grpc=/Users/Vladislav.Artiukhov/.local/bin/grpc_cpp_plugin service.proto
