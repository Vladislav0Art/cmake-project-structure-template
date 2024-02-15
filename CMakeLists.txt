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


message(NOTICE "Binary dir: " ${CMAKE_CURRENT_BINARY_DIR})
message(NOTICE "Components srcs: " ${COMPONENTS_SRCS})
message(NOTICE "Utils srcs: " ${UTILS_SRCS})


add_executable(main
    src/main.cpp
    ${COMPONENTS_SRCS}
    ${UTILS_SRCS}
)


target_include_directories(main PUBLIC
    ./include
    ${CMAKE_CURRENT_BINARY_DIR}
)
