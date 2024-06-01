cmake_minimum_required(VERSION 3.18)

project(SampleCmakeProject CXX)

# include auxiliary cmake file grpc sources generation
# provides 'generated_proto_models_srcs' variable which contains generated source files of proto models
include(./cmake/grpc-generation.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_subdirectory(./include/)

#### variables propagations
# COMPONENTS_SRCS       - propagated from ./include/components
# SERVICES_SRCS         - propagated from ./include/services
message(NOTICE "COMPONENTS_SRCS: " ${COMPONENTS_SRCS})
message(NOTICE "SERVICES_SRCS: " ${SERVICES_SRCS})

# requires grpc and protobuf to be locally installed:
# if the installation is not present in the path then add the following argument into the cmake configuration:
# -DCMAKE_PREFIX_PATH=path/to/grpc/installation (e.g., -DCMAKE_PREFIX_PATH=/Users/Vladislav.Artiukhov/.local)
# if VSCode CMake extention is used, navigate to: "Open Settings UI" -> type cmake -> scroll down to "Cmake: Configure Args" -> Add item "-DCMAKE_PREFIX_PATH=/Users/Vladislav.Artiukhov/.local"
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)


# Creating executable
add_executable(main
    src/main.cpp

    ${COMPONENTS_SRCS}
    ${SERVICES_SRCS}

    ${generated_proto_models_srcs}
)

add_dependencies(main generate_proto_models)

target_link_libraries(main
    absl::flags_parse
    gRPC::grpc++
    protobuf::libprotobuf
)

target_include_directories(main PUBLIC
    ./include
    ./libs
    ${CMAKE_CURRENT_BINARY_DIR}
)


# create test executable
add_executable(test
    tests/client.cpp
    tests/doctest_main.cpp

    ${COMPONENTS_SRCS}
    ${SERVICES_SRCS}

    ${generated_proto_models_srcs}
)

add_dependencies(test generate_proto_models)

target_link_libraries(test
    absl::flags_parse
    gRPC::grpc++
    protobuf::libprotobuf
)

target_include_directories(test PUBLIC
    ./include
    ./libs
    ${CMAKE_CURRENT_BINARY_DIR}
)