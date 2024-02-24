cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProject CXX)

#### setting global props
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#### adding subdirs
add_subdirectory(./include/)

#### variables propagations
# COMPONENTS_SRCS       - propagated from ./include/components
# UTILS_SRCS            - propagated from ./include/utils
message(NOTICE "Binary dir: " ${CMAKE_CURRENT_BINARY_DIR})
message(NOTICE "Components srcs: " ${COMPONENTS_SRCS})
message(NOTICE "Utils srcs: " ${UTILS_SRCS})


# If gRPC is locally installed, add into cmake: `-DCMAKE_PREFIX_PATH=path/to/installation` (mine is: /Users/Vladislav.Artiukhov/.local)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)


set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>) # "/Users/Vladislav.Artiukhov/.local/bin/protoc"
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>) # "/Users/Vladislav.Artiukhov/.local/bin/grpc_cpp_plugin"

message(NOTICE "_PROTOBUF_PROTOC: " ${_PROTOBUF_PROTOC})
message(NOTICE "_GRPC_CPP_PLUGIN_EXECUTABLE: " ${_GRPC_CPP_PLUGIN_EXECUTABLE})

# Proto files directory
get_filename_component(proto_files_dir "./proto" ABSOLUTE)
message(NOTICE "proto_files_dir: " ${proto_files_dir})


# Contains all the files generated inside build folder ending with pb.cc (i.e. grpc and proto cc-files)
file(GLOB _proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/*.pb.cc")
set(proto_srcs ${_proto_srcs})
# Contains all the files generated inside build folder ending with pb.h (i.e. grpc and proto h-files)
file(GLOB _proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/*.pb.h")
set(proto_hdrs ${_proto_hdrs})



message(NOTICE "proto_srcs: " ${proto_srcs})
message(NOTICE "proto_hdrs: " ${proto_hdrs})

add_custom_command(
    OUTPUT "${proto_srcs}" "${proto_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${proto_files_dir}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${proto_files_dir}/*"
    DEPENDS "${proto_files_dir}"
)

# Creates a custom target which is dependant on the grpc/proto models generation
add_custom_target(
  generate_proto_and_grpc_sources_target
  DEPENDS "${proto_srcs}" "${proto_hdrs}"
)

add_executable(main
    src/main.cpp
    ${COMPONENTS_SRCS}
    ${UTILS_SRCS}

    ${proto_srcs}
    ${proto_hdrs}
)

# Adds proto generation as a dependency of the executable
add_dependencies(main generate_proto_and_grpc_sources_target)


target_include_directories(main PUBLIC
    ./include
    ./libs
    ${CMAKE_CURRENT_BINARY_DIR}
    # ${Protobuf_INCLUDE_DIRS}
    # ./build/service.grpc.pb.cc
    # ./build/service.pb.cc
    # ./build/service.grpc.pb.h
    # ./build/service.pb.h
)

# Generate gRPC services inside ./buide folder via Protobuf and gRPC plugin:
# /Users/Vladislav.Artiukhov/.local/bin/protoc --grpc_out=./build --cpp_out=./build --plugin=protoc-gen-grpc=/Users/Vladislav.Artiukhov/.local/bin/grpc_cpp_plugin service.proto
# /Users/Vladislav.Artiukhov/my-libraries/grpc/cmake/build

# /Users/Vladislav.Artiukhov/.local/bin/protoc --grpc_out=./build --cpp_out=./build --plugin=protoc-gen-grpc=/Users/Vladislav.Artiukhov/my-libraries/grpc/cmake/build/grpc_cpp_plugin service.proto

target_link_libraries(main
    # absl::flags
    absl::flags_parse
    gRPC::grpc++
    protobuf::libprotobuf
    # Threads::Threads
    # gRPC::grpc++_reflection
    # ${Protobuf_LIBRARIES}
)